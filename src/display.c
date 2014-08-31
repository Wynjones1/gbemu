#include "display.h"
#include "events.h"
#include "common.h"
#include "cpu.h"
#include "opcodes.h"
#include "debug.h"
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

unsigned char g_video_data[DISPLAY_HEIGHT][DISPLAY_WIDTH];

struct display
{
	SDL_Window   *window;
	SDL_Renderer *render;
	SDL_Texture  *texture;
	cpu_state_t  *state;
	memory_t     *mem;
	//Thread Data
	pthread_t thread;

	unsigned char debug_data[DISPLAY_HEIGHT][DEBUG_REGISTER_WIDTH][4];

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

	TTF_SetFontStyle(d->font,   0);
	TTF_SetFontOutline(d->font, 0);
	TTF_SetFontKerning(d->font, 0);
	TTF_SetFontHinting(d->font, 0);

	SDL_Color fg    = {255, 255, 255};
	d->surface      = TTF_RenderText_Solid(d->font, "1234512345", fg);
	d->font_texture = SDL_CreateTextureFromSurface(d->render, d->surface);
}

static void init_display(display_t *display)
{
	display->window = SDL_CreateWindow("Window", 0, 0, 
									PIXEL_SCALE * (DISPLAY_WIDTH + DEBUG_REGISTER_WIDTH + DEBUG_INSTRUCTION_WIDTH),
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
								SDL_TEXTUREACCESS_STREAMING,
								DISPLAY_WIDTH + DEBUG_REGISTER_WIDTH + DEBUG_INSTRUCTION_WIDTH, DISPLAY_HEIGHT);
	if(!display->texture)
	{
		Error("%s\n", SDL_GetError());
	}
	if(SDL_SetRenderDrawColor(display->render, 0xff, 0xff, 0xff, 0xff) < 0)
	{
		Error("%s\n", SDL_GetError());
	}
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
	g_state = display->state;
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

static void write_pixel(uint8_t *data, int pitch, int x, int y, uint8_t col)
{
	data[pitch * y + 4 * x + 0] = col;
	data[pitch * y + 4 * x + 1] = col;
	data[pitch * y + 4 * x + 2] = col;
	data[pitch * y + 4 * x + 3] = 255;
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

void transfer_buffer(display_t *d)
{
	uint8_t *data;
	int      pitch;
	const SDL_Rect rect = {.x = 0, .y = 0,.w = DISPLAY_WIDTH, .h = DISPLAY_HEIGHT};
	SDL_LockTexture(d->texture, &rect, (void*)&data, &pitch);
	for(int j = 0; j < DISPLAY_HEIGHT; j++)
	{
		for(int i = 0; i < DISPLAY_WIDTH; i++)
		{
			int shade = GET_SHADE(d->mem->bgp, g_video_data[j][i]);
			data[pitch * j + 4 * i + 0] = shade;
			data[pitch * j + 4 * i + 1] = shade;
			data[pitch * j + 4 * i + 2] = shade;
		}
	}
	SDL_UnlockTexture(d->texture);
}

static void write_sprites(display_t *display)
{
	reg_t *video_ram = display->mem->video_ram;
	const struct OAM_data *data;
	uint8_t *pixel_data;
	int pitch;
	const SDL_Rect rect = {.x = 0, .y = 0,.w = DISPLAY_WIDTH, .h = DISPLAY_HEIGHT};
	SDL_LockTexture(display->texture, &rect, (void*)&pixel_data, &pitch);
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
				uint8_t x = x_pos;
				if(data->x_flip)
				{
					x += i;
				}
				else
				{
					x += (7 - i);
				}
				uint8_t y = y_pos;
				if(data->y_flip)
				{
					y += (7 - j);
				}
				else
				{
					y += j;
				}
				if(x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT)
				{
					if(shade != 0)
					{
						write_pixel(pixel_data, pitch, x, y , GET_SHADE(display->mem->bgp, shade));
					}
				}
			}
			tile_data += 2;
		}
	}
	SDL_UnlockTexture(display->texture);
}

void display_display(display_t *display)
{
	//Display the image.
	if(1)//display->mem->lcdc.enabled)
	{
		transfer_buffer(display);
		//write_background(display);
		//write_window(display);
		write_sprites(display);
		display_present(display);
	}
	else
	{
		display_clear(display);
	}
}

void display_clear(display_t *disp)
{
	SDL_RenderClear(disp->render);
	SDL_RenderPresent(disp->render);
}

void draw_line(display_t *disp, const char *buf, int line, int column, int width)
{
	int h = 18;
	SDL_Rect debug_rect = {
		.x = DISPLAY_WIDTH, .y = 0, 
		.w = DEBUG_REGISTER_WIDTH  , .h = DISPLAY_HEIGHT
	};
	SDL_Rect temp = {
		.x = PIXEL_SCALE * (DISPLAY_WIDTH + DEBUG_REGISTER_WIDTH * column)  , .y = h * line,
		.w = PIXEL_SCALE * width , .h = h 
	};

	SDL_Color fg = {255, 255, 255};
	disp->surface = TTF_RenderText_Solid(disp->font, buf, fg);
	disp->font_texture = SDL_CreateTextureFromSurface(disp->render, disp->surface);

	if(SDL_UpdateTexture(disp->texture, &debug_rect, disp->debug_data,
							PIXEL_SIZE * DEBUG_REGISTER_WIDTH) < 0)
	{
		Error("%s\n", SDL_GetError());
	}

	if(SDL_RenderCopy(disp->render, disp->font_texture, NULL, &temp) < 0)
	{
		Error("%s\n", SDL_GetError());
	}

	SDL_DestroyTexture(disp->font_texture);
	SDL_FreeSurface(disp->surface);
}

void draw_instructions(display_t *display)
{
/*
	char buf[1024];
	char buf0[1024];
	uint16_t addr = display->state->pc;
	struct opcode *op = &op_table[cpu_load8(display->state, addr)];
	debug_print_op(buf0, display->state, op);
	sprintf(buf, " | %-25s", buf0);
	draw_line(display, buf, 0, 1, DEBUG_INSTRUCTION_WIDTH);
*/
}

void draw_debug(display_t *disp)
{
	draw_instructions(disp);
	char buf[1024];
	sprintf(buf, "PC    = 0x%04x", disp->state->pc);
	draw_line(disp, buf, 0, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "AF    = 0x%04x", disp->state->af);
	draw_line(disp, buf, 1, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "BC    = 0x%04x", disp->state->bc);
	draw_line(disp, buf, 2, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "DE    = 0x%04x", disp->state->de);
	draw_line(disp, buf, 3, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "HL    = 0x%04x", disp->state->hl);
	draw_line(disp, buf, 4, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "SP    = 0x%04x", disp->state->sp);
	draw_line(disp, buf, 5, 0, DEBUG_REGISTER_WIDTH);

	sprintf(buf, "SCX   = 0x%04x" , disp->state->memory->scx);
	draw_line(disp, buf, 6, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "SCY   = 0x%04x"  , disp->state->memory->scy);
	draw_line(disp, buf, 7, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "LY    = 0x%04x" , disp->state->memory->ly);
	draw_line(disp, buf, 8, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "LXC   = 0x%04x"  , disp->state->memory->lyc);
	draw_line(disp, buf, 9, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "WX    = 0x%04x" , disp->state->memory->wx);
	draw_line(disp, buf, 10, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "WY    = 0x%04x", disp->state->memory->wy);
	draw_line(disp, buf, 11, 0, DEBUG_REGISTER_WIDTH);

	sprintf(buf, "Bank  = 0x%04x", disp->state->memory->current_bank);
	draw_line(disp, buf, 12, 0, DEBUG_REGISTER_WIDTH);

	sprintf(buf, "LCDC:         ");
	draw_line(disp, buf, 13, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "BG Display : %u", disp->state->memory->lcdc.bg_display);
	draw_line(disp, buf, 14, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "OBJ Enable : %u", disp->state->memory->lcdc.obj_enable);
	draw_line(disp, buf, 15, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "OBJ Size   : %u", disp->state->memory->lcdc.obj_size);
	draw_line(disp, buf, 16, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "Map Select : %u", disp->state->memory->lcdc.map_select);
	draw_line(disp, buf, 17, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "Tile Select: %u", disp->state->memory->lcdc.tile_select);
	draw_line(disp, buf, 18, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "Window Disp: %u", disp->state->memory->lcdc.window_display);
	draw_line(disp, buf, 19, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "Window Map : %u", disp->state->memory->lcdc.window_map);
	draw_line(disp, buf, 20, 0, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "Enabled    : %u", disp->state->memory->lcdc.enabled);
	draw_line(disp, buf, 21, 0, DEBUG_REGISTER_WIDTH);


	sprintf(buf, "Interrupt Flags (val/enabled):");
	draw_line(disp, buf, 0, 1, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "IME      : %u  ", disp->state->memory->IME);
	draw_line(disp, buf, 1, 1, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "VBLANK   : %u/%u",
						disp->state->memory->interrupt.v_blank,
						disp->state->memory->enabled.v_blank);
	draw_line(disp, buf, 2, 1, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "LCD STAT : %u/%u",
						disp->state->memory->interrupt.lcd_status,
						disp->state->memory->enabled.lcd_status);
	draw_line(disp, buf, 3, 1, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "TIMER    : %u/%u",
						disp->state->memory->interrupt.timer,
						disp->state->memory->enabled.timer);
	draw_line(disp, buf, 4, 1, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "SERIAL   : %u/%u",
						disp->state->memory->interrupt.serial,
						disp->state->memory->enabled.serial);
	draw_line(disp, buf, 5, 1, DEBUG_REGISTER_WIDTH);
	sprintf(buf, "JOYPAD   : %u/%u",
						disp->state->memory->interrupt.joypad,
						disp->state->memory->enabled.joypad);
	draw_line(disp, buf, 6, 1, DEBUG_REGISTER_WIDTH);

	sprintf(buf, "TIMA/TMA/EN   : %u/%u/%u",
						disp->state->memory->tima,
						disp->state->memory->tma,
						disp->state->memory->tac.enable);
	draw_line(disp, buf, 7, 1, DEBUG_REGISTER_WIDTH);
}

//TODO:Change to need DEBUG flag.
void display_present(display_t *disp)
{
	SDL_Rect rect = {
		.x = 0, .y = 0,
		.w = DISPLAY_WIDTH, .h = DISPLAY_HEIGHT
	};
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
