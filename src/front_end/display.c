#include "display.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>

#define PIXEL_SIZE  4
#define PIXEL_SCALE 3

struct display
{
	SDL_Window   *window;
	SDL_Renderer *render;
	SDL_Texture  *texture;

	unsigned char pixel_data[DISPLAY_HEIGHT][DISPLAY_WIDTH][4];
};

display_t *display_init(void)
{
	display_t *out = malloc(sizeof(display_t));
	SDL_Init(SDL_INIT_VIDEO);
	out->window = SDL_CreateWindow("Window", 0, 0, 
									PIXEL_SCALE * DISPLAY_WIDTH,
									PIXEL_SCALE * DISPLAY_HEIGHT, 0);
	out->render  = SDL_CreateRenderer(out->window, -1, 0);
	out->texture = SDL_CreateTexture(out->render,
								SDL_PIXELFORMAT_ARGB8888,
								SDL_TEXTUREACCESS_STATIC,
								DISPLAY_WIDTH, DISPLAY_HEIGHT);
	memset(out->pixel_data, 0x00, sizeof(out->pixel_data));
	display_present(out);
	return out;
}

void display_delete(display_t *disp)
{
	SDL_DestroyWindow(disp->window);
	free(disp);
}

void display_draw_pixel(display_t *disp, int x, int y, char *rgb)
{
	memcpy(disp->pixel_data[x][y], rgb, PIXEL_SIZE);
	display_present(disp);
}

void display_present(display_t *disp)
{
	SDL_UpdateTexture(disp->texture, NULL, disp->pixel_data, PIXEL_SIZE * DISPLAY_WIDTH);
	SDL_RenderClear(disp->render);
	SDL_RenderCopy(disp->render, disp->texture, NULL, NULL);
	SDL_RenderPresent(disp->render);
}
