#include "platform.h"
#include "audio.h"
#include "logging.h"

#if PLATFORM_SDL
void fill_audio(void *udata, Uint8 *stream, int len);

void setup_audio(audio_t *audio)
{
	SDL_AudioSpec wanted;
	/* Set the audio format */
	wanted.freq     = FREQUENCY;
	wanted.format   = AUDIO_S16;
	wanted.channels = NUM_CHANNELS;
	wanted.samples  = NUM_SAMPLES;
	wanted.callback = fill_audio;
	wanted.userdata = audio;

	/* Open the audio device, forcing the desired format */
	if (SDL_OpenAudio(&wanted, NULL) < 0)
	{
		log_error("Couldn't open audio: %s\n", SDL_GetError());
	}
	SDL_PauseAudio(0);
}

void delay(uint32_t ms)
{
	SDL_Delay(ms);
}

uint32_t get_ticks(void)
{
	return SDL_GetTicks();
}


void write_framebuffer(display_t *display)
{
	SDL_Error(SDL_RenderClear(display->render) < 0);

	// Write the framebuffer onto the the texture.
	SDL_Rect screen = { 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT };
	// If the display is LCD is disabled display a white background.
	if (display->mem->lcdc.enabled)
	{
		SDL_UpdateTexture(display->texture, &screen, display->draw_buffer, DISPLAY_WIDTH * sizeof(uint32_t));
	}
	else
	{
		SDL_UpdateTexture(display->texture, &screen, white_buffer, DISPLAY_WIDTH * sizeof(uint32_t));
	}

#if CONTROLS
	SDL_Rect controls = { 0, DISPLAY_HEIGHT, DISPLAY_WIDTH, CONTROLS_HEIGHT };
	SDL_UpdateTexture(display->texture, &controls, control_buffer, DISPLAY_WIDTH * sizeof(uint32_t));
#endif

	// Draw the framebuffer texture onto the screen.
	SDL_Rect output = { 0, 0, PIXEL_SCALE * DISPLAY_WIDTH, PIXEL_SCALE * (DISPLAY_HEIGHT + CONTROLS_HEIGHT) };
	SDL_Error(SDL_RenderCopy(display->render, display->texture, NULL, &output) < 0);

#if DEBUG_WINDOW
	text_area_draw(display->text_area);
#endif

	SDL_RenderPresent(display->render);
}
#endif