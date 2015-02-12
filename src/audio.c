#include "cpu.h"
#include "audio.h"
#include "memory.h"
#include <pthread.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define NUM_CHANNELS 2
#define NUM_SAMPLES  1024
#define FREQUENCY    44100
#define VOLUME       0.005

void audio_start_thread(void)
{
}

static audio_t *g_audio;

typedef struct
{
    int16_t left;
    int16_t right;
}sample_t;

#define FREQ(x) (CPU_CLOCK_SPEED / (32 * ((float)(2048 - (x)))))
static int16_t square(float t, float freq, int volume)
{
    float val  = sin(freq * 2 * PI * t);
    int16_t out;
    if(val < 0.0)
    {
        out = -(INT16_MAX / 10);
    }
    else
    {
        out = INT16_MAX / 10;
    }
    return (volume * out) / 15;
}
static int16_t square1(float t)
{
    float freq = FREQ(g_audio->sq1.freq_msb << 8 | g_audio->sq1.freq_lsb);
    int16_t out = square(t, freq, g_audio->sq1.volume);
    return out;
}

static int16_t square2(float t)
{
    float freq = FREQ(g_audio->sq2.freq_msb << 8 | g_audio->sq2.freq_lsb);
    int16_t out = square(t, freq, g_audio->sq2.volume);
    return out;
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
        samples[i]= (sample_t){.left = val, .right = val};
        pos = (pos + 1) % FREQUENCY;
    }
}

audio_t *audio_init(cpu_state_t *state)
{
	SDL_AudioSpec wanted;
	audio_t *out = (audio_t*) calloc(1, sizeof(audio_t));
	out->state   = state;
    g_audio = out;

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
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		return NULL;
	}
	SDL_PauseAudio(0);
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
    }
    return 0;
}


void audio_delete(audio_t *audio)
{
	free(audio);
}

void length_counter(audio_t *audio)
{
    if(audio->sq1.load_len && audio->sq1.len_en)
    {
        audio->sq1.load_len -= 1;
        if(audio->sq1.load_len == 0) audio->sq1.en = 0;
    }
    if(audio->sq2.load_len && audio->sq2.len_en)
    {
        audio->sq2.load_len -= 1;
        if(audio->sq2.load_len == 0) audio->sq2.en = 0;
    }
}

void sweep(audio_t *audio)
{
    uint16_t shadow   = audio->sq1.freq_msb << 8 | audio->sq1.freq_lsb;
    uint16_t new_freq = 0;
    static uint16_t count = 0;
    if(audio->sq1.sweep_period)
    {
        count++;
        if(count >= audio->sq1.sweep_period)
        {
            count = 0;
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

void envelope(audio_t *audio)
{
    {
        static int counter = 0;
        if(audio->sq1.en && audio->sq1.period)
        {
            if(counter >= audio->sq1.period)
            {
                counter = 0;
                if(audio->sq1.add_mode)
                {
                    audio->sq1.volume += 1;
                    if(audio->sq1.volume > 15) audio->sq1.volume = 15;
                }
                else
                {
                    audio->sq1.volume -= 1;
                    if(audio->sq1.volume < 0) audio->sq1.volume = 0;
                }
            }
            counter++;
        }
        else
        {
            counter = 0;
        }
    }
    {
        static int counter = 0;
        if(audio->sq2.en && audio->sq2.period)
        {
            if(counter >= audio->sq2.period)
            {
                counter = 0;
                if(audio->sq2.add_mode)
                {
                    audio->sq2.volume += 1;
                    if(audio->sq2.volume > 15) audio->sq2.volume = 15;
                }
                else
                {
                    audio->sq2.volume -= 1;
                    if(audio->sq2.volume < 0) audio->sq2.volume = 0;
                }
            }
            counter++;
        }
        else
        {
            counter = 0;
        }
    }
}

void frame_sequencer(audio_t *audio, int clk)
{
    static int step  = 0;
    static int count = 0;
    const int period = CPU_CLOCK_SPEED / 512;
    count = count + clk;
    if(count > period)
    {
        count -= period;
        step = (step + 1) % 8;
        if(step % 2 == 0)
        {
            //Length Ctrl
            length_counter(audio);
            if(step == 2 || step == 6)
            {
                sweep(audio);
            }
        }
        if(step == 7)
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
    //printf("%04X ", addr); common_print_binary(stdout, data, 8); printf("\n");
    #define X(field, msb, lsb) audio->sq1.field = DECODE(data, msb, lsb);
    switch(addr)
    {
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
    }
}
