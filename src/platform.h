#ifndef PLATFORM_H
#define PLATFORM_H

#define PLATFORM_SDL 0

#if PLATFORM_SDL
	#include "SDL.h"
	#include "SDL_ttf.h"
	#define HAVE_TTF 1
#else
	#include <stdint.h>
	typedef uint8_t Uint8;
	#define HAVE_TTF 0
#endif

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct audio   audio_t;
typedef struct display display_t;

void     setup_audio(audio_t *audio);
void     delay(uint32_t ms);
uint32_t get_ticks(void);

void write_framebuffer(display_t *display);

#ifdef __cplusplus
	} // extern "C"
#endif

#endif