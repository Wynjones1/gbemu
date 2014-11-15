#include "audio.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <SDL.h>

void audio_start_thread(void)
{
}

extern void fill_audio(void *udata, Uint8 *stream, int len)
{
}

SDL_AudioSpec wanted;
audio_t *audio_init(cpu_state_t *state)
{
	extern void fill_audio(void *udata, Uint8 *stream, int len);
	audio_t *out = (audio_t*) malloc(sizeof(audio_t));
	out->state   = state;

	/* Set the audio format */
	wanted.freq = 22050;
	wanted.format = AUDIO_S16;
	wanted.channels = 2;    /* 1 = mono, 2 = stereo */
	wanted.samples = 1024;  /* Good low-latency value for callback */
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
//	fprintf(stderr, "Audio load\n");
	return 0;
}

void  audio_store(audio_t *audio, reg16_t addr, reg_t data)
{
//	fprintf(stderr, "Audio store\n");
}

void audio_delete(audio_t *audio)
{
	SDL_CloseAudio();
	free(audio);
}
