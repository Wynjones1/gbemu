#include "cpu.h"
#include "audio.h"
#include "logging.h"
#include "memory.h"
#include <assert.h>
#include <stdlib.h>

#define NUM_CHANNELS 2
#define NUM_SAMPLES  1024
#define FREQUENCY    44100
#define VOLUME       0.005

static audio_t *g_audio;


typedef struct
{
    int16_t left;
    int16_t right;
}sample_t;

#define FREQ(msb, lsb) (CPU_CLOCK_SPEED / (32 * ((float)(2048 - (msb << 8 | lsb)))))

static uint8_t duty_table[4][8] =
{
    {0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,1,1},
    {0,1,1,1,1,1,1,0},
};
static int16_t square(float t, float freq, int volume, int duty)
{
	int idx = (int)(t * freq * 8);
    int16_t val = duty_table[duty][idx % 8] ? INT16_MAX : INT16_MIN;
    val = volume * val / (15 * 40);
    return val;
}
static int16_t square1(float t)
{
    float freq = FREQ(g_audio->sq1.freq_msb, g_audio->sq1.freq_lsb);
    int16_t out = square(t, freq, g_audio->sq1.volume, g_audio->sq1.duty);
    return out;
}

static int16_t square2(float t)
{
    float freq = FREQ(g_audio->sq2.freq_msb,  g_audio->sq2.freq_lsb);
    int16_t out = square(t, freq, g_audio->sq2.volume, g_audio->sq2.duty);
    return out;
}

static int16_t wave(float t)
{
    float freq = 440;
	int idx = (int)(t * freq * 64);
    uint8_t sample = g_audio->wave_table[idx >> 1];
    if(idx & 0x1) return sample & MASK(4);
    return INT16_MAX * (sample >> 4) / 15;
}

static void fill_audio(void *udata, Uint8 *stream, int len)
{
    static int pos = 0;
    sample_t *samples = (sample_t*)stream;
    int num_samples = len / (sizeof(sample_t));
    for(int i = 0; i < num_samples; i++)
    {
        float t = pos / (float)FREQUENCY;
        int16_t val = 0;
        if(g_audio->sq1.en)
        {
            val += square1(t);
        }
        if(g_audio->sq2.en)
        {
            val += square2(t);
        }
#if 0
        if(g_audio->wave.en)
        {
            val += wave(t);
        }
#endif
        samples[i]= (sample_t){.left = val, .right = val};
        pos = (pos + 1) % FREQUENCY;
    }
}

audio_t *audio_init(cpu_state_t *state)
{
	audio_t *out = CALLOC(1, sizeof(audio_t));
	out->state   = state;
    g_audio = out;

#if AUDIO
	SDL_AudioSpec wanted;
	/* Set the audio format */
	wanted.freq     = FREQUENCY;
	wanted.format   = AUDIO_S16;
	wanted.channels = NUM_CHANNELS;
	wanted.samples  = NUM_SAMPLES;
	wanted.callback = fill_audio;
	wanted.userdata = NULL;

	/* Open the audio device, forcing the desired format */
	if ( SDL_OpenAudio(&wanted, NULL) < 0 )
	{
        log_error("Couldn't open audio: %s\n", SDL_GetError());
	}
	SDL_PauseAudio(0);
#endif
	return out;
}


reg_t audio_load(audio_t *audio, reg16_t addr)
{
    switch(addr)
    {
    #define X(field, msb, lsb) ENCODE(audio->sq1.field, msb, lsb) |
        case 0xff10: return (NR10 0x80);
        case 0xff11: return (NR11 0x3f);
        case 0xff12: return (NR12 0x00);
        case 0xff13: return (NR13 0xff);
        case 0xff14: return (NR14 0xbf);
    #undef X
    #define X(field, msb, lsb) ENCODE(audio->sq2.field, msb, lsb) |
        case 0xff15: return 0xff;
        case 0xff16: return (NR21 0x3f);
        case 0xff17: return (NR22 0x00);
        case 0xff18: return (NR23 0xff);
        case 0xff19: return (NR24 0xbf);
    #undef X
    #define X(field, msb, lsb) ENCODE(audio->wave.field, msb, lsb) |
        case 0xff1a: return (NR30 0x7f);
        case 0xff1b: return (NR31 0xff);
        case 0xff1c: return (NR32 0x9f);
        case 0xff1d: return (NR33 0xff);
        case 0xff1e: return (NR34 0xbf);
    #undef X
    #define X(field, msb, lsb) ENCODE(audio->noise.field, msb, lsb) |
        case 0xff1f: return 0xff;
        case 0xff20: return (NR41 0xff);
        case 0xff21: return (NR42 0x00);
        case 0xff22: return (NR43 0x00);
        case 0xff23: return (NR44 0xbf);
    #undef X
    }
    return 0;
}


void audio_delete(audio_t *audio)
{
	free(audio);
}

void length_counter(audio_t *audio)
{
#define X(FIELD)                                    \
    if(audio->FIELD.load_len && audio->FIELD.len_en)\
	{                                               \
		audio->FIELD.load_len -= 1;                 \
		if (audio->FIELD.load_len == 0)             \
			audio->FIELD.en = 0;                    \
	}

	X(sq1);
	X(sq2);
	X(wave);
#undef X
}

void sweep(audio_t *audio)
{
    uint16_t shadow   = audio->sq1.freq_msb << 8 | audio->sq1.freq_lsb;
    uint16_t new_freq = 0;
    if(audio->sq1.sweep_period)
    {
        audio->sweep_count++;
        if(audio->sweep_count >= audio->sq1.sweep_period)
        {
			audio->sweep_count = 0;
            if(audio->sq1.shift > 0 || audio->sq1.sweep_period > 0)
            {
                new_freq = (shadow >> audio->sq1.shift);
                new_freq = audio->sq1.negate ? -new_freq : new_freq;
                new_freq = shadow + new_freq;
                if(new_freq >> 11) audio->sq1.en = 0;
                audio->sq1.freq_lsb = DECODE(new_freq, 7, 0);
                audio->sq1.freq_msb = DECODE(new_freq, 10, 8);
            }
        }
    }
}

static void envelope_channel(uint8_t en, uint8_t period, uint8_t add_mode, int8_t *volume, uint8_t *counter)
{
    if(en && period)
    {
        if(*counter >= period)
        {
            *counter = 0;
            if(add_mode)
            {
                *volume += 1;
                if(*volume > 15) *volume = 15;
            }
            else
            {
                *volume -= 1;
                if(*volume == -1) *volume = 0;
            }
        }
        *counter += 1;
    }
    else
    {
        *counter = 0;
    }
}
void envelope(audio_t *audio)
{
    static uint8_t counter[4];
    envelope_channel(audio->sq1.en, audio->sq1.period, audio->sq1.add_mode, &audio->sq1.volume, counter + 0);
    envelope_channel(audio->sq2.en, audio->sq2.period, audio->sq2.add_mode, &audio->sq2.volume, counter + 1);
}

void frame_sequencer(audio_t *audio, int clk)
{
    const uint32_t period = CPU_CLOCK_SPEED / 512;
    audio->frame_sequencer_count += clk;
    if(audio->frame_sequencer_count >= period)
    {
		audio->frame_sequencer_count -= period;
		audio->frame_sequencer_step += 1;
		audio->frame_sequencer_step &= 0x7;
        if(audio->frame_sequencer_step % 2 == 0)
        {
            length_counter(audio);
            if(audio->frame_sequencer_step == 2 ||
			   audio->frame_sequencer_step == 6)
            {
                sweep(audio);
            }
        }
        if(audio->frame_sequencer_step == 7)
        {
            envelope(audio);
        }
    }
}

void audio_simulate(audio_t *audio, int clk)
{
    frame_sequencer(audio, clk);
}

void  audio_store(audio_t *audio, reg16_t addr, reg_t data)
{
    switch(addr)
    {
    #define X(field, msb, lsb) audio->sq1.field = DECODE(data, msb, lsb);
        case 0xff10:
            NR10;
            break;
        case 0xff11:
            NR11;
            audio->sq1.load_len = 64 - audio->sq1.load_len;
            break;
        case 0xff12:
            NR12;
            break;
        case 0xff13:
            NR13;
            break;
        case 0xff14:
            NR14;
            if(DECODE(data, 7, 7))
            {
                audio->sq1.en = 1;
                audio->sq1.volume = audio->sq1.start_volume;
            }
            break;
    #undef X
    #define X(field, msb, lsb) audio->sq2.field = DECODE(data, msb, lsb);
        case 0xff16:
            NR21;
            audio->sq2.load_len = 64 - audio->sq2.load_len;
            break;
        case 0xff17:
            NR22;
            break;
        case 0xff18:
            NR23;
            break;
        case 0xff19:
            NR24;
            if(DECODE(data, 7, 7))
            {
                audio->sq2.en = 1;
                audio->sq2.volume = audio->sq2.start_volume;
            }
            break;
    #undef X
    #define X(field, msb, lsb) audio->wave.field = DECODE(data, msb, lsb);
        case 0xff1a: NR30; break;
        case 0xff1b: NR31; break;
        case 0xff1c: NR32; break;
        case 0xff1d: NR33; break;
        case 0xff1e:
            NR34;
            if(DECODE(data, 7, 7))
            {
                audio->wave.en = 1;
            }
            break;
    #undef X
    #define X(field, msb, lsb) audio->noise.field = DECODE(data, msb, lsb);
        case 0xff20: NR41; break;
        case 0xff21: NR42; break;
        case 0xff22: NR43; break;
        case 0xff23: NR44; break;
    #undef X
    #define X(field, msb, lsb) audio->control.field = DECODE(data, msb, lsb);
        case 0xff24: NR50; break;
        case 0xff25: NR51; break;
        case 0xff26: NR52; break;
    #undef X
        //Wave table.
        case 0xff30: case 0xff31: case 0xff32: case 0xff33:
        case 0xff34: case 0xff35: case 0xff36: case 0xff37:
        case 0xff38: case 0xff39: case 0xff3a: case 0xff3b:
        case 0xff3c: case 0xff3d: case 0xff3e: case 0xff3f:
            audio->wave_table[addr - 0xff30] = data;
            break;
        default:
            log_warning("Write to invalid sound register 0x%04X 0x%04X.\n", addr, data);
            break;
    }
}
