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

static uint8_t volume = 15;
static int16_t square1(float t)
{
    int   freq = CPU_CLOCK_SPEED / (4 * 2 * (2048 - g_audio->square1.freq));
    float val  = sin(freq * 2 * PI * t);
    float v    = VOLUME * (volume / 15.0);
    if(val < 0.0)
    {
        return v * INT16_MIN;
    }
    else
    {
        return v * INT16_MAX;
    }
}

static int16_t square2(float t)
{
    return 0;
    int   freq = g_audio->square2.freq;
    float val  = sin(freq * 2 * PI * t);
    if(val < 0.0)
    {
        return VOLUME * INT16_MIN;
    }
    else
    {
        return VOLUME * INT16_MAX;
    }
}

static int16_t noise(float t)
{
    return 0;
}

static int16_t wave(float t)
{
    return 0;
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
        if(g_audio->channel_en[0]) {
            val += square1(t);
        }
        if(g_audio->channel_en[1]) {
            val += square2(t);
        }
        if(g_audio->channel_en[2]) {
            val += wave(t);
        }
        if(g_audio->channel_en[3]) {
            val += noise(t);
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
	return audio->registers[addr - 0xff10];
}

void  audio_store(audio_t *audio, reg16_t addr, reg_t data)
{
	audio->registers[addr - 0xff10] = data;
    if(addr == 0xff10)
    {
    }
    if(addr == 0xff11)
    {
        audio->channel_counters[0] = audio->square1.len_load;
    }
    if(addr == 0xff12)
    {
        volume = audio->square1.volume;
    }
    if(addr == 0xff14 && (data & (1 << 7)))
    {
        audio->channel_en[0] = 1;
        if(audio->channel_counters[0] == 0)
            audio->channel_counters[0] = 64;
    }
}

void audio_delete(audio_t *audio)
{
	free(audio);
}

void length_counter(audio_t *audio)
{
    if(audio->channel_counters[0] && audio->square1.len_en)
    {
        audio->channel_counters[0] -= 1;
        if(audio->channel_counters[0] == 0)
            audio->channel_en[0] = 0;
    }
}

void sweep(audio_t *audio)
{
    uint16_t shadow   = audio->square1.freq;
    uint16_t new_freq = 0;
    static uint16_t count = 0;
    if(audio->square1.sweep)
    {
        count++;
        if(count >= audio->square1.sweep)
        {
            count = 0;
            if(audio->square1.shift > 0 || audio->square1.sweep > 0)
            {
                audio->channel_counters[0] = audio->square1.len_load;
                new_freq = (shadow >> audio->square1.shift);
                new_freq = audio->square1.negate ? -new_freq : new_freq;
                new_freq = shadow + new_freq;
                if(new_freq >> 11) audio->channel_en[0] = 0;
                audio->square1.freq = new_freq;
            }
        }
    }
}

void envelope(audio_t *audio)
{
    static int count = 0;
    count++;
    if(count >= audio->square1.period)
    {
        count = 0;
        if(audio->square1.period)
        {
            if(audio->square1.env_add)
            {
                volume = min(volume + 1, 15);
            }
            else
            {
                volume = max(volume - 1, 0);
            }
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
    int period;
    frame_sequencer(audio, clk);
    if(audio->square1.freq)
    {
        period = CPU_CLOCK_SPEED / audio->square1.freq;
        audio->channel_counters[0] = (audio->channel_counters[0] + clk) % period;
        if(audio->channel_counters[0] < clk)
        {
            //Decrement Square1 counter.
        }
    }
}
