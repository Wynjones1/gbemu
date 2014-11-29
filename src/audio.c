#include "audio.h"
#include <pthread.h>
#include <stdlib.h>
#include <SDL.h>

void audio_start_thread(void)
{
}
#define PI 3.1415926
#define min(x, y)\
		((x) < (y) ? (x) : (y))

extern void fill_audio(void *udata, Uint8 *stream, int len)
{
}

audio_t *audio_init(cpu_state_t *state)
{
	SDL_AudioSpec wanted;
	extern void fill_audio(void *udata, Uint8 *stream, int len);
	audio_t *out = (audio_t*) malloc(sizeof(audio_t));
	out->state   = state;

	/* Set the audio format */
	wanted.freq = 22050;
	wanted.format = AUDIO_S16;
	wanted.channels = 2;    /* 1 = mono, 2 = stereo */
	wanted.samples = 256;  /* Good low-latency value for callback */
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
}

void audio_delete(audio_t *audio)
{
	free(audio);
}
