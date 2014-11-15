#include "display.h"
#include "events.h"
#include "cpu.h"
#include "opcodes.h"
#include "debug.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
//TODO: Remove pthreads
#include <pthread.h>

const int PIXEL_SIZE  = 4;
const int PIXEL_SCALE = 2;
const int NUMBER_OF_OAM_ELEMENTS = 40;

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
	//TTF Data
	TTF_Font     *font;
	SDL_Surface  *surface;
	unsigned char debug_data[DISPLAY_HEIGHT][DEBUG_REGISTER_WIDTH][4];
};

static void *display_thread(void *display_);

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

	SDL_Color fg= {255, 255, 255};
	d->surface  = TTF_RenderText_Solid(d->font, " ", fg);
}

static void delete_ttf(display_t *d)
{
	TTF_CloseFont(d->font);
}

pthread_cond_t g_init_cond = PTHREAD_COND_INITIALIZER;

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

	if(DEBUG)
	{
		memset(display->debug_data, 0x00, sizeof(display->debug_data));
		init_ttf(display);
	}
}

display_t *display_init(cpu_state_t *state)
{
	display_t *display = (display_t*) malloc(sizeof(display_t));
	display->state  = state;
	display->mem    = state->memory;
	SDL_Init(SDL_INIT_VIDEO);
	if(DISPLAY_ENABLED)
	{
		if(DISPLAY_THREAD)
		{
			pthread_t thread;
			pthread_create(&thread, NULL, display_thread, display);
			pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
			pthread_cond_wait(&g_init_cond, &mtx);
		}
		else
		{
			init_display(display);
		}
	}
	atexit(SDL_VideoQuit);
	atexit(SDL_Quit);
	return display;
}

static void *display_thread(void *display_)
{
	display_t *display = (display_t*) display_;
	init_display(display);
	pthread_cond_signal(&g_init_cond);
	g_state = display->state;
	pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_wait(&g_state->start_cond, &mtx);
	while(1)
	{
		events_handle(display->state);
		display_display(display);
		SDL_Delay(17);
	}
	return NULL;
}

void display_delete(display_t *disp)
{
	if(DEBUG)
	{
		delete_ttf(disp);
	}
	SDL_DestroyTexture(disp->texture);
	SDL_DestroyRenderer(disp->render);
	SDL_DestroyWindow(disp->window);
	free(disp);
}

void transfer_buffer(display_t *d)
{
	uint8_t *data;
	int      pitch;
	const SDL_Rect rect = {.x = 0, .y = 0,.w = DISPLAY_WIDTH, .h = DISPLAY_HEIGHT};
	SDL_LockTexture(d->texture, &rect, (void**)&data, &pitch);
	for(int j = 0; j < DISPLAY_HEIGHT; j++)
	{
		for(int i = 0; i < DISPLAY_WIDTH; i++)
		{
			int shade = g_video_data[j][i];
			data[pitch * j + 4 * i + 0] = shade;
			data[pitch * j + 4 * i + 1] = shade;
			data[pitch * j + 4 * i + 2] = shade;
		}
	}
	SDL_UnlockTexture(d->texture);
}

void display_display(display_t *display)
{
	//Display the image.
	if(display->mem->lcdc.enabled)
	{
		transfer_buffer(display);
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

	SDL_Color fg             = {255, 255, 255};
	SDL_Surface *surface      = TTF_RenderText_Solid(disp->font, buf, fg);
	SDL_Texture *font_texture = SDL_CreateTextureFromSurface(disp->render, disp->surface);

	if(SDL_UpdateTexture(disp->texture, &debug_rect, disp->debug_data,
							PIXEL_SIZE * DEBUG_REGISTER_WIDTH) < 0)
	{
		Error("%s\n", SDL_GetError());
	}

	if(SDL_RenderCopy(disp->render, font_texture, NULL, &temp) < 0)
	{
		Error("%s\n", SDL_GetError());
	}

	SDL_DestroyTexture(font_texture);
	SDL_FreeSurface(surface);
}

void draw_instructions(display_t *display)
{
	char buf[1024];
	char buf0[1024];
	uint16_t addr = display->state->pc;
	const struct opcode *op = &op_table[cpu_load8(display->state, addr)];
	debug_print_op(buf0, display->state, op);
	sprintf(buf, " | %-25s", buf0);
	draw_line(display, buf, 0, 1, DEBUG_INSTRUCTION_WIDTH);
}

void draw_debug(display_t *disp)
{
	char buf[1024];
	draw_instructions(disp);
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
	sprintf(buf, "Tile Select: %u", disp->state->memory->lcdc.tile_data_select);
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

void display_present(display_t *disp)
{
	if(SDL_RenderClear(disp->render) < 0)
	{
		Error("%s\n", SDL_GetError());
	}
	if(SDL_RenderCopy(disp->render, disp->texture, NULL, NULL) < 0)
	{
		Error("%s\n", SDL_GetError());
	}
	if(DEBUG)
	{
		draw_debug(disp);
	}
	SDL_RenderPresent(disp->render);
}

uint8_t display_get_shade(const uint8_t *tile_data, int i)
{
	return ((tile_data[1] >> (7 - (i))) & 0x1) << 1 | ((tile_data[0] >> (7 - (i))) & 0x1);
}

static int get_sprite_shade(struct cpu_state *state, struct OAM_data *sprite, int x, int y)
{
	int ox = x - (sprite->x_pos - 8);
	int oy = y - (sprite->y_pos - 16);
	int second_tile = 0;
	int size = state->memory->lcdc.obj_size ? 16 : 8;
	if(sprite->x_flip)
	{
		ox = 7 - ox;
	}
	if(sprite->y_flip)
	{
		oy = size - oy - 1;
	}
	if(oy > 7)
	{
		oy -= 8;
		second_tile = 1;
	}
	uint8_t *tile_data = state->memory->video_ram + sprite->tile * 16 + 2 * oy + 16 * second_tile;
	return display_get_shade(tile_data, ox);
}

int g_shade;
static struct OAM_data *get_sprite(struct cpu_state *state, int x, int y)
{
	struct OAM_data *sprite, *out = NULL;
	uint8_t size = state->memory->lcdc.obj_size ? 16 : 8;
	for(int i = 0; i < NUMBER_OF_OAM_ELEMENTS; i++)
	{
		sprite = state->memory->oam_data + i;
		int x_pos = sprite->x_pos - 8;
		int y_pos = sprite->y_pos - 16;
		if( (x_pos <= x && x < x_pos + 8) && (y_pos <= y && y < y_pos + size) )
		{
			int shade = get_sprite_shade(state, sprite, x, y);
			if(shade)
			{
				//TODO:Remove the global state.
				g_shade = shade;
				out = sprite;
			}
		}
	}
	return out;
}

uint8_t shade_table[4] =
{
	255,
	127,
	63,
	0
};

//TODO: Properly comment this.
#define GET_SHADE(n, x) shade_table[((x >> (2 * n)) & 0x3)]

static void write_sprites(struct cpu_state *state, int x)
{
	int y = state->memory->ly;
	uint8_t *data = g_video_data[y];
	struct OAM_data *sprite = get_sprite(state, x, y);
	if(sprite)
	{
		uint8_t palette = sprite->palette ? state->memory->obp1 : state->memory->obp0;
		data[x] = GET_SHADE(g_shade, palette);
	}
}
/* Write the current line that is drawing into the framebuffer */
static void write_background(struct cpu_state *state, int x)
{
	int ty        = (state->memory->ly + state->memory->scy) / 8;
	int offset    = (state->memory->ly + state->memory->scy) % 8;
	int scx = state->memory->scx;
	uint8_t *data = g_video_data[state->memory->ly];
	const uint8_t *tile_data;
	int tx = ((x + scx) / 8) % 0x20;
	int ox = (x + scx) % 8;
	tile_data = memory_get_tile_data(state->memory, tx, ty, offset, state->memory->lcdc.map_select);
	data[x] = GET_SHADE(display_get_shade(tile_data, ox), state->memory->bgp);
}

static void write_window(struct cpu_state *state, int x)
{
	int wx = state->memory->wx - 7;
	int wy = state->memory->wy;
	if(state->memory->lcdc.window_display && state->memory->ly > wy && wx <= x)
	{
		uint8_t *data = g_video_data[state->memory->ly];
		int tx = (x - wx) / 8;
		int ox = (x - wx) % 8;
		int ty = (state->memory->ly - wy) / 8;
		int oy = (state->memory->ly - wy) % 8;
		const uint8_t *tile_data = memory_get_tile_data(state->memory, tx, ty, oy, state->memory->lcdc.window_map);
		data[x] = GET_SHADE(display_get_shade(tile_data, ox), state->memory->bgp);
	}
}


static void write_display(struct cpu_state *state)
{
	if(state->memory->ly < DISPLAY_HEIGHT)
	{
		for(int i = 0; i < DISPLAY_WIDTH; i++)
		{
			write_background(state, i);
			write_window(state, i);
			write_sprites(state, i);
		}
	}
}

void display_simulate(struct cpu_state *state)
{
	if(state->clock_counter >= CPU_CLOCKS_PER_LINE) //This should take 16ms
	{
		state->clock_counter -= CPU_CLOCKS_PER_LINE;
		state->memory->ly = (state->memory->ly + 1) % 154;
		if(state->memory->ly == state->memory->lyc)
		{
			state->memory->stat.coincidence = 1;
			if(state->memory->stat.coincidence_int)
			{
				state->memory->interrupt.lcd_status = 1;
			}
		}
		else
		{
			state->memory->stat.coincidence = 0;
		}

		if(state->memory->ly == 144)
		{
			state->memory->interrupt.v_blank = 1;
		}
		write_display(state);
	}
}
