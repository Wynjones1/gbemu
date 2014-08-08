#include "display.h"
#include "events.h"
#include "common.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
//TODO: Remove pthreads
#include <pthread.h>

#define PIXEL_SIZE  4
#define PIXEL_SCALE 3

static void *display_thread(void *display_);

struct display
{
	SDL_Window   *window;
	SDL_Renderer *render;
	SDL_Texture  *texture;
	memory_t     *mem;
	//Thread Data
	pthread_t thread;

	unsigned char pixel_data[DISPLAY_HEIGHT][DISPLAY_WIDTH][4];
};

display_t *display_init(memory_t *mem)
{
	display_t *out = malloc(sizeof(display_t));
	out->mem    = mem;
	pthread_create(&out->thread, NULL, display_thread, out);
	return out;
}

void display_delete(display_t *disp)
{
	SDL_DestroyWindow(disp->window);
	free(disp);
}

static void write_pixel(display_t *d, int x, int y, uint8_t col)
{
	d->pixel_data[y][x][0] = col;
	d->pixel_data[y][x][1] = col;
	d->pixel_data[y][x][2] = col;
	d->pixel_data[y][x][3] = 255;
}

uint8_t shade_table[4] =
{
	255,
	127,
	63,
	0
};

//TODO: Properly comment this.
#define GET_SHADE(x, n) shade_table[((x >> (2 * n)) & 0x3)]

static void write_tile(display_t *d, int tx, int ty)
{
	//Tile map is located at address 0x9800 or 0x9c00
	int tile_num = ty * 32 + tx;
	uint8_t  tile = d->mem->video_ram[0x1800 + tile_num];
	uint8_t *tile_data = &d->mem->video_ram[tile * 16];
	uint8_t scx = d->mem->scx;
	uint8_t scy = d->mem->scy;
	for(int j = 0; j < 8; j++)
	{
		for(int i = 0; i < 8; i++)
		{
			uint8_t shade = ((tile_data[1] >> i) & 0x1) << 1 |
							((tile_data[0] >> i) & 0x1);
			uint8_t x = tx * 8 + (7 - i);
			uint8_t y = ty * 8 + j;
			x = x - scx;
			y = y - scy;
			if(x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT)
			{
				write_pixel(d, x, y , GET_SHADE(d->mem->bgp, shade));
			}
		}
		tile_data += 2;
	}
}

static void *display_thread(void *display_)
{
	display_t *display = display_;
	SDL_Init(SDL_INIT_VIDEO);

	display->window = SDL_CreateWindow("Window", 0, 0, 
									PIXEL_SCALE * DISPLAY_WIDTH,
									PIXEL_SCALE * DISPLAY_HEIGHT, 0);
	display->render  = SDL_CreateRenderer(display->window, -1, 0);
	display->texture = SDL_CreateTexture(display->render,
								SDL_PIXELFORMAT_ARGB8888,
								SDL_TEXTUREACCESS_STATIC,
								DISPLAY_WIDTH, DISPLAY_HEIGHT);
	memset(display->pixel_data, 0x00, sizeof(display->pixel_data));

	events_t events;
	memset(&events, 0x00, sizeof(events_t));
	while(1)
	{
		//Check for events.
		events_handle(&events);
		if(events.quit) exit(0);
		//Display the image.
		for(int ty = 0; ty < 32; ty++)
		{
			for(int tx = 0; tx < 32; tx++)
			{
				write_tile(display, tx, ty);
			}
		}
		display_present(display);
		SDL_Delay(17);
	}
	return NULL;
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
