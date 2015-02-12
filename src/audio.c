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
static int16_t square1(float t)
{
    float freq = FREQ(g_audio->freq);
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
#if 0
    static FILE *fp;
    if(!fp) fp = fopen("plot.txt", "w");
    fprintf(fp, "%f %f\n", SDL_GetTicks() / 1000.0, val / (float)(INT16_MAX));
#endif
    return (g_audio->volume * out) / 15;
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
        if(g_audio->en)
        {
            val += square1(t);
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
    return 0;
}


void audio_delete(audio_t *audio)
{
	free(audio);
}

void length_counter(audio_t *audio)
{
    if(audio->load_len && audio->len_en)
    {
        audio->load_len -= 1;
        if(audio->load_len == 0) audio->en = 0;
    }
}

void sweep(audio_t *audio)
{
#if 1
    uint16_t shadow   = audio->freq;
    uint16_t new_freq = 0;
    static uint16_t count = 0;
    if(audio->sweep_period)
    {
        count++;
        if(count >= audio->sweep_period)
        {
            count = 0;
            if(audio->shift > 0 || audio->sweep_period > 0)
            {
                new_freq = (shadow >> audio->shift);
                new_freq = audio->negate ? -new_freq : new_freq;
                new_freq = shadow + new_freq;
                if(new_freq >> 11) audio->en = 0;
                audio->freq = new_freq;
            }
        }
    }
#else
#endif
}

void envelope(audio_t *audio)
{
    static int counter = 0;
    if(audio->en && audio->period)
    {
        if(counter >= audio->period)
        {
            counter = 0;
            if(audio->add_mode)
            {
                audio->volume += 1;
                if(audio->volume > 15) audio->volume = 15;
            }
            else
            {
                audio->volume -= 1;
                if(audio->volume < 0) audio->volume = 0;
            }
        }
        counter++;
    }
    else
    {
        counter = 0;
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
    if(addr == 0xff10)
    {
        audio->shift        = DOWNTO(data, 2, 0);
        audio->negate       = DOWNTO(data, 3, 3);
        audio->sweep_period = DOWNTO(data, 6, 4);
    }
    if(addr == 0xff11)
    {
        audio->load_len = 64 - DOWNTO(data, 5, 0);
        audio->duty     = DOWNTO(data, 7, 6);
    }
    if(addr == 0xff12)
    {
        audio->period        = DOWNTO(data, 2, 0);
        audio->add_mode      = DOWNTO(data, 3, 3);
        audio->start_volume  = DOWNTO(data, 7, 4);
    }
    if(addr == 0xff13)
    {
        audio->freq = (DOWNTO(audio->freq, 10, 8) << 8) | data;
    }
    if(addr == 0xff14)
    {
        audio->freq   = (DOWNTO(data, 2, 0) << 8) | DOWNTO(audio->freq, 7, 0);
        audio->len_en = DOWNTO(data, 6, 6);
        if(BIT_N(data, 7))
        {
            audio->en = 1;
            audio->volume = audio->start_volume;
        }
    }
}
