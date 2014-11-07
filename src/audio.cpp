#include "audio.h"
#include <pthread.h>

void audio_start_thread(void)
{
	
}

extern void fill_audio(void *udata, Uint8 *stream, int len)
{
	for(int i = 0; i < len; i++)
		stream[i] = rand() % 1;
}

int audio_init(void)
{
	SDL_AudioSpec wanted;
	extern void fill_audio(void *udata, Uint8 *stream, int len);

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
		return -1;
	}
	SDL_PauseAudio(0);
	return 0;
}
