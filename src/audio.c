#include "cpu.h"
#include "audio.h"
#include "logging.h"
#include "memory.h"
#include "noise_table.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"

// Parameters for the SDL audio device.
#define NUM_CHANNELS 2
#define NUM_SAMPLES  1024
#define FREQUENCY    (44100)
#define VOLUME       0.005

typedef struct
{
    int16_t left;
    int16_t right;
}sample_t;


static inline float calc_freq(uint32_t freq_msb, uint32_t freq_lsb)
{
	uint32_t freq = freq_msb << 8 | freq_lsb;
	freq = 2048 - freq;
	freq <<= 5;
	return CPU_CLOCK_SPEED / (float)freq;
}

static uint8_t square(float t, uint8_t freq_msb, uint8_t freq_lsb, int volume, int duty)
{
	const uint8_t duty_table[4][8] =
	{
		{0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1},
		{1,0,0,0,0,1,1,1},
		{0,1,1,1,1,1,1,0},
	};

	float    freq = calc_freq(freq_msb, freq_lsb);
	uint32_t idx  = (uint32_t)(t * freq * 8);
	return volume * duty_table[duty][idx % 8];
}

static uint8_t square1(float t, audio_t *audio)
{
    return square(t, audio->sq1.freq_msb, audio->sq1.freq_lsb, audio->sq1.volume, audio->sq1.duty);
}

static uint8_t square2(float t, audio_t *audio)
{
    return square(t, audio->sq2.freq_msb, audio->sq2.freq_lsb, audio->sq2.volume, audio->sq2.duty);
}

static uint8_t wave(float t, audio_t *audio)
{
	const uint8_t shift_table[] =
	{
		4, 0, 1, 2
	};

	// The wave frequency is half that of the other channels.
	float freq = calc_freq(audio->wave.freq_msb, audio->wave.freq_lsb) / 2.0f;
	uint32_t idx    = (uint32_t)(t * freq * 32);
    uint8_t  sample = audio->wave_table[idx % 32];
	uint8_t  shift  = shift_table[audio->wave.volume_code];
	uint8_t  value  = sample >> shift;
	return value;
}

static uint32_t noise_freq(audio_t *audio)
{
	/* Information found here:
	   http://belogic.com/gba/channel4.shtml
	*/
	const uint32_t noise_divisor_table[15][8] =
	{
#define X(SHIFT) {                                 \
		(CPU_CLOCK_SPEED / (8 / 2)) >> (SHIFT + 1),\
		(CPU_CLOCK_SPEED / (8 * 1)) >> (SHIFT + 1),\
		(CPU_CLOCK_SPEED / (8 * 2)) >> (SHIFT + 1),\
		(CPU_CLOCK_SPEED / (8 * 3)) >> (SHIFT + 1),\
		(CPU_CLOCK_SPEED / (8 * 4)) >> (SHIFT + 1),\
		(CPU_CLOCK_SPEED / (8 * 5)) >> (SHIFT + 1),\
		(CPU_CLOCK_SPEED / (8 * 6)) >> (SHIFT + 1),\
		(CPU_CLOCK_SPEED / (8 * 7)) >> (SHIFT + 1)},

		X(0)  X(1)  X(2)
		X(3)  X(4)  X(5)
		X(6)  X(7)  X(8)
		X(9)  X(10) X(11)
		X(12) X(13) X(14)
#undef X
	};

	uint32_t freq = noise_divisor_table[audio->noise.clock_shift][audio->noise.divisor];
	return freq;
}

static inline uint8_t sample_noise(float t, uint32_t freq, const uint8_t *data, size_t data_size)
{
#if 0
	uint32_t idx = (uint32_t)(t * freq * data_size);
	uint32_t num_samples = freq / FREQUENCY;
	int32_t out = 0;
	if (num_samples == 0) num_samples = 1;
	int32_t sum = 0;
	for (int i = 0; i < num_samples; i++)
	{
		int j = idx - num_samples / 2 + i;
		sum += (data[j % data_size] ? INT16_MAX : INT16_MIN);
	}
	out = sum / num_samples;
	return out;
#else
	uint32_t idx = (uint32_t)(t * freq * data_size);
	return data[idx % data_size];
#endif
}

static uint8_t noise(float t, audio_t *audio)
{
	uint32_t freq = noise_freq(audio);
	uint8_t out;
	if (audio->noise.width_mode)
	{
		out = sample_noise(t, freq, noise_table_7, sizeof(noise_table_7));
	}
	else
	{
		out = sample_noise(t, freq, noise_table_15, sizeof(noise_table_15));
	}
	return out * audio->noise.volume;
}

static inline int16_t DAC(uint8_t x)
{
	return (int16_t)(INT16_MAX * (((float)(2 * x) / 15.0f) - 1.0f));
}

static void fill_audio(void *udata, Uint8 *stream, int len)
{
    sample_t *samples = (sample_t*)stream;
	audio_t *audio = (audio_t*)udata;
    int num_samples = len / (sizeof(sample_t));
    for(int i = 0; i < num_samples; i++)
    {
        float t = audio->buffer_pos / (float)FREQUENCY;
        int16_t val_l = 0;
        int16_t val_r = 0;
        uint8_t channels[4];
#if 1
        if(audio->sq1.en)
        {
            channels[0] = square1(t, audio);
        }

        if(audio->sq2.en)
        {
            channels[1] = square2(t, audio);
        }

        if(audio->wave.en && audio->wave.power)
        {
            channels[2] = wave(t, audio);
        }
#endif

#if 1
		if (audio->noise.en)
		{
			channels[3] = noise(t, audio);
		}
#endif

        for(int i = 0; i < 4; i++)
        {
            int16_t v = DAC(channels[i]) / 4;
            if(BIT_N(audio->control.r_en, i))
            {
                val_r += v;
            }
            if(BIT_N(audio->control.l_en, i))
            {
                val_l += v;
            }
        }

		samples[i] = (sample_t) { .left = val_l / 4, .right = val_r / 4 };
		audio->buffer_pos = (audio->buffer_pos + 1) % FREQUENCY;
    }
}

static void init_wave_table(audio_t *audio)
{
	const uint8_t wave_default_table[32] =
	{
		0x4, 0x8, 0x0, 0x4, 0x3, 0x4,0xA, 0xA,
		0xD, 0x2, 0x8, 0x7, 0x2, 0x9,0xC, 0x3,
		0x0, 0x6, 0x9, 0x5, 0x9, 0x5,0x0, 0xB,
		0x4, 0x3, 0x8, 0xB, 0xE, 0x2,0xA, 0xD,
	};
	memcpy(audio->wave_table, wave_default_table, sizeof(audio->wave_table));
}

audio_t *audio_init(cpu_state_t *state)
{
	audio_t *out = CALLOC(1, sizeof(audio_t));
	out->state   = state;

	init_wave_table(out);

#if AUDIO
	SDL_AudioSpec wanted;
	/* Set the audio format */
	wanted.freq     = FREQUENCY;
	wanted.format   = AUDIO_S16;
	wanted.channels = NUM_CHANNELS;
	wanted.samples  = NUM_SAMPLES;
	wanted.callback = fill_audio;
	wanted.userdata = out;

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

void audio_store_wave_sample(audio_t *audio, uint8_t idx, reg_t samples)
{
	// Two samples are packed into a single byte, extract them.
	audio->wave_table[idx * 2]     = samples >> 4; // The first sample is in the MSB
	audio->wave_table[idx * 2 + 1] = samples & 0xf;
}

void length_counter(audio_t *audio)
{
#define X(FIELD, BIT)                                  \
    if(audio->FIELD.load_len && audio->FIELD.len_en)   \
	{                                                  \
		audio->FIELD.load_len -= 1;                    \
		if (audio->FIELD.load_len == 0){               \
			audio->FIELD.en = 0;                       \
            RESET_N(audio->control.channel_stat, BIT); \
        }                                              \
	}

	X(sq1, 0);
	X(sq2, 1);
	X(wave, 2);
	X(noise, 3);
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

static inline void envelope_channel(uint8_t en, uint8_t period, uint8_t add_mode, int8_t *volume, uint8_t *counter)
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

void envelope(audio_t *a)
{
    #define X(FIELD, BIT) envelope_channel(                \
                           a->FIELD.en,                    \
                           a->FIELD.period,                \
                           a->FIELD.add_mode,              \
                           &a->FIELD.volume,               \
                           &a->FIELD.envelope_counter);
    X(sq1,   0);
    X(sq2,   1);
    X(noise, 3);
    #undef X
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

static void power_control(audio_t *audio)
{
	if (audio->control.power == 0)
	{
		/* Zero the memory of the sound registers. The subtraction
		   in the memset's are to account for non-register state
		   that needs to be maintained.
		*/
		memset(&audio->sq1,     0x00, sizeof(audio->sq1)     - 3);
		memset(&audio->sq2,     0x00, sizeof(audio->sq2)     - 3);
		memset(&audio->wave,    0x00, sizeof(audio->wave)    - 2);
		memset(&audio->noise,   0x00, sizeof(audio->noise)   - 3);
		memset(&audio->control, 0x00, sizeof(audio->control) - 2);
		audio->frame_sequencer_count = 0;
	}
}

void audio_simulate(audio_t *audio, int clk)
{
	power_control(audio);
    frame_sequencer(audio, clk);
}

void  audio_store(audio_t *audio, reg16_t addr, reg_t data)
{
    switch(addr)
    {
		// Sound 1 Registers
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
	// Sound 2 Registers
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
		// Wave Registers
		#define X(field, msb, lsb) audio->wave.field = DECODE(data, msb, lsb);
        case 0xff1a: NR30; break;
        case 0xff1b:
			NR31;
			audio->wave.load_len = 256 - audio->wave.load_len;
			break;
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
		// Noise Registers
		#define X(field, msb, lsb) audio->noise.field = DECODE(data, msb, lsb);
        case 0xff20:
			NR41;
			audio->noise.load_len = 64 - audio->noise.load_len;
			break;
        case 0xff21: NR42; break;
        case 0xff22: NR43; break;
        case 0xff23:
			NR44;
			if (DECODE(data, 7, 7))
			{
				audio->noise.en = 1;
				audio->noise.volume = audio->noise.start_volume;
			}
			break;
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
			audio_store_wave_sample(audio, addr - 0xff30, data);
            break;
		case 0xff27: case 0xff28: case 0xff29:
		case 0xff2a: case 0xff2b: case 0xff2c:
		case 0xff2d: case 0xff2e: case 0xff2f:
			break;
        default:
            log_warning("Write to invalid sound register 0x%04X 0x%04X.\n", addr, data);
            break;
    }
}
