#include "cpu.h"
#include "audio.h"
#include "memory.h"
#include <pthread.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define NUM_CHANNELS 2
#define NUM_SAMPLES  1024
#define FREQUENCY    44100
#define VOLUME       0.01

void audio_start_thread(void)
{
}

static audio_t *g_audio;

typedef struct
{
    int16_t left;
    int16_t right;
}sample_t;

extern void fill_audio(void *udata, Uint8 *stream, int len)
{
    static int pos = 0;
    sample_t *samples = (sample_t*)stream;
    int num_samples = len / (sizeof(sample_t));
    int freq = g_audio->square1.freq;
    if(g_audio->channel_en[0])
    {
        for(int i = 0; i < num_samples; i++)
        {
            float t = pos / (float)FREQUENCY;
            int max = INT16_MAX;
            float val = max * sin(freq * 2 * PI * t);
            if(val < 0.0)
            {
                samples[i].left  = VOLUME * INT16_MIN;
                samples[i].right = VOLUME * INT16_MIN;
            }
            else
            {
                samples[i].left  = VOLUME * INT16_MAX;
                samples[i].right = VOLUME * INT16_MAX;
            }
            pos = (pos + 1) % FREQUENCY;
        }
    }
}

audio_t *audio_init(cpu_state_t *state)
{
	SDL_AudioSpec wanted;
	extern void fill_audio(void *udata, Uint8 *stream, int len);
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
        audio->channel_counters[0] = audio->square1.len_load;
        audio->channel_en[0] = 1;
    }
}

void audio_delete(audio_t *audio)
{
	free(audio);
}

void length_counter(audio_t *audio)
{
    if(audio->channel_counters[0])
    {
        audio->channel_counters[0] -= 1;
        if(audio->channel_counters[0] == 0)
            audio->channel_en[0] = 0;
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
                //Sweep
            }
        }
        if(step == 7)
        {
            //Vol Env
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
