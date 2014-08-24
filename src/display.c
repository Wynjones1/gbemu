#include "display.h"
#include "events.h"
#include "common.h"
#include "cpu.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
//TODO: Remove pthreads
#include <pthread.h>

#define PIXEL_SIZE  4
#define PIXEL_SCALE 3
#define DISPLAY_ENABLED 1
#define NUMBER_OF_OAM_ELEMENTS 40

struct display
{
	SDL_Window   *window;
	SDL_Renderer *render;
	SDL_Texture  *texture;
	cpu_state_t  *state;
	memory_t     *mem;
	//Thread Data
	pthread_t thread;

	unsigned char pixel_data[DISPLAY_HEIGHT][DISPLAY_WIDTH][4];
	unsigned char debug_data[DISPLAY_HEIGHT][DEBUG_WIDTH][4];

	//TTF Data
	TTF_Font *font;
	SDL_Surface *surface;
	SDL_Texture *font_texture;
};

#if DISPLAY_ENABLED
#if DISPLAY_THREAD
static void *display_thread(void *display_);
#endif

static void init_ttf(display_t *d)
{
	if(TTF_Init() < 0)
	{
		Error("%s\n", SDL_GetError());
	}
	d->font = TTF_OpenFont("./data/fonts/font0.ttf", 102);
	if(d->font == NULL)
	{
		Error("%s\n", SDL_GetError());
	}
	TTF_SetFontStyle(d->font, 0);
	TTF_SetFontOutline(d->font, 0);
	TTF_SetFontKerning(d->font, 0);
	TTF_SetFontHinting(d->font, 0);
	SDL_Color fg = {255, 255, 255};
	d->surface = TTF_RenderText_Solid(d->font, "1234512345", fg);
	d->font_texture = SDL_CreateTextureFromSurface(d->render, d->surface);
}

static void init_display(display_t *display)
{
	display->window = SDL_CreateWindow("Window", 0, 0, 
									PIXEL_SCALE * (DISPLAY_WIDTH + DEBUG_WIDTH),
									PIXEL_SCALE * DISPLAY_HEIGHT, 0);
	if(!display->window)
	{
		Error("%s\n", SDL_GetError());
	}
	display->render  = SDL_CreateRenderer(display->window, -1, 0);
	if(!display->render)
	{
		Error("%s\n", SDL_GetError());
	}
	display->texture = SDL_CreateTexture(display->render,
								SDL_PIXELFORMAT_ARGB8888,
								SDL_TEXTUREACCESS_STATIC,
								DISPLAY_WIDTH + DEBUG_WIDTH, DISPLAY_HEIGHT);
	if(!display->texture)
	{
		Error("%s\n", SDL_GetError());
	}
	if(SDL_SetRenderDrawColor(display->render, 0xff, 0xff, 0xff, 0xff) < 0)
	{
		Error("%s\n", SDL_GetError());
	}
	memset(display->pixel_data, 0x00, sizeof(display->pixel_data));
	memset(display->debug_data, 0x00, sizeof(display->debug_data));

	init_ttf(display);
}
#endif

display_t *display_init(cpu_state_t *state)
{
	display_t *display = malloc(sizeof(display_t));
	display->state  = state;
	display->mem    = state->memory;
#if DISPLAY_ENABLED
	SDL_Init(SDL_INIT_VIDEO);
#if DISPLAY_THREAD
	pthread_t thread;
	pthread_create(&thread, NULL, display_thread, display);
#else
	init_display(display);
#endif
#endif
	return display;
}

#if DISPLAY_THREAD && DISPLAY_ENABLED
void handle_events(struct cpu_state *state);
static void *display_thread(void *display_)
{
	display_t *display = display_;
	init_display(display);
	while(1)
	{
		events_handle(display->state);
		display_display(display);
		SDL_Delay(17);
	}
	return NULL;
}
#endif

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
	uint8_t  tile = d->mem->video_ram[(d->mem->lcdc.map_select ? 0x1c00 : 0x1800) + tile_num];
	//Tils data is located at addresses
	// 0x8800 -> 97FF or
	// 0x8000 -> 8FFF
	uint8_t *tile_data = &d->mem->video_ram[(d->mem->lcdc.tile_select ? 0x000 : 0x800) +  tile * 16];
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

static void write_background(display_t *display)
{
	for(int ty = 0; ty < 32; ty++)
	{
		for(int tx = 0; tx < 32; tx++)
		{
			write_tile(display, tx, ty);
		}
	}
}

static void write_window(display_t *display)
{
}

static void write_sprites(display_t *display)
{
	reg_t *video_ram = display->mem->video_ram;
	const struct OAM_data *data;
	for(int e = 0; e < NUMBER_OF_OAM_ELEMENTS; e++)
	{
		data = display->mem->oam_data + e;
		uint8_t x_pos = data->x_pos - 8;
		uint8_t y_pos = data->y_pos - 16;
		uint8_t *tile_data = video_ram + data->tile * 16;
		for(int j = 0; j < 8; j++)
		{
			for(int i = 0; i < 8; i++)
			{
				uint8_t shade = ((tile_data[1] >> i) & 0x1) << 1 |
								((tile_data[0] >> i) & 0x1);
				uint8_t x = x_pos + (7 - i);
				uint8_t y = y_pos + j;
				if(x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT)
				{
					write_pixel(display, x, y , GET_SHADE(display->mem->bgp, shade));
				}
			}
			tile_data += 2;
		}
	}
}

void display_display(display_t *display)
{
	//Display the image.
	if(display->mem->lcdc.enabled)
	{
		write_background(display);
		write_window(display);
		write_sprites(display);
		display_present(display);
	}
	else
	{
		display_clear(display);
	}
}

void display_draw_pixel(display_t *disp, int x, int y, char *rgb)
{
	memcpy(disp->pixel_data[x][y], rgb, PIXEL_SIZE);
}

void display_clear(display_t *disp)
{
	SDL_RenderClear(disp->render);
	SDL_RenderPresent(disp->render);
}

void draw_line(display_t *disp, const char *buf, int line)
{
	SDL_Rect debug_rect = {
		.x = DISPLAY_WIDTH, .y = 0, 
		.w = DEBUG_WIDTH  , .h = DISPLAY_HEIGHT
	};
	SDL_Rect temp = {
		.x = PIXEL_SCALE * DISPLAY_WIDTH, .y = 30 * line,
		.w = PIXEL_SCALE * DEBUG_WIDTH, .h = 30 
	};

	SDL_Color fg = {255, 255, 255};
	disp->surface = TTF_RenderText_Solid(disp->font, buf, fg);
	disp->font_texture = SDL_CreateTextureFromSurface(disp->render, disp->surface);

	if(SDL_UpdateTexture(disp->texture, &debug_rect, disp->debug_data, PIXEL_SIZE * DEBUG_WIDTH) < 0)
	{
		Error("%s\n", SDL_GetError());
	}

	if(SDL_RenderCopy(disp->render, disp->font_texture, NULL, &temp) < 0)
	{
		Error("%s\n", SDL_GetError());
	}
	
}
void draw_debug(display_t *disp)
{
/*
	char buf[1024];
	sprintf(buf, "PC = 0x%04x", disp->state->pc);
	draw_line(disp, buf, 0);
	sprintf(buf, "SP = 0x%04x", disp->state->sp);
	draw_line(disp, buf, 1);
	sprintf(buf, "AF = 0x%04X", disp->state->af);
	draw_line(disp, buf, 2);
	sprintf(buf, "BC = 0x%04X", disp->state->bc);
	draw_line(disp, buf, 3);
	sprintf(buf, "DE = 0x%04X", disp->state->de);
	draw_line(disp, buf, 4);
*/
}

//TODO:Change to need DEBUG flag.
void display_present(display_t *disp)
{
	SDL_Rect rect = {
		.x = 0, .y = 0,
		.w = DISPLAY_WIDTH, .h = DISPLAY_HEIGHT
	};
	if(SDL_UpdateTexture(disp->texture, &rect, disp->pixel_data, PIXEL_SIZE * DISPLAY_WIDTH) < 0)
	{
		Error("%s\n", SDL_GetError());
	}
	if(SDL_RenderClear(disp->render) < 0)
	{
		Error("%s\n", SDL_GetError());
	}
	if(SDL_RenderCopy(disp->render, disp->texture, NULL, NULL) < 0)
	{
		Error("%s\n", SDL_GetError());
	}
	draw_debug(disp);
	SDL_RenderPresent(disp->render);
}
