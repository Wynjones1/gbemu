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

static const int PIXEL_SIZE  = 4;
static const int NUMBER_OF_OAM_ELEMENTS = 40;
static int PIXEL_SCALE = 0;

static void *display_thread(void *display_);
static void draw_debug(display_t *disp);

unsigned char g_video_data[DISPLAY_HEIGHT][DISPLAY_WIDTH];
char instruction_buffer[200];
char last_instruction[200];

#define SDL_Error(cond)                       \
	if(cond)                                  \
		Error("%s\n", SDL_GetError());        \

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
	SDL_Texture *font_texture;
	SDL_Surface *surface;
};

static SDL_Surface *RenderText(TTF_Font *font, const char *string)
{
    static const int mode = 2;
    static const SDL_Color fg = {255, 255, 255};
    static const SDL_Color bg = {0, 0, 0};
    if(mode == 1)
    {
        return TTF_RenderText_Shaded(font, string, fg, bg);
    }
    else if(mode == 2)
    {
        return TTF_RenderText_Blended(font, string, fg);
    }
    return TTF_RenderText_Solid(font, string, fg);
}

static void init_ttf(display_t *d)
{
	SDL_Error(TTF_Init() < 0);

	d->font = TTF_OpenFont("./data/fonts/font0.ttf", 102);
	SDL_Error(d->font == NULL);

	TTF_SetFontStyle(d->font,   0);
	TTF_SetFontOutline(d->font, 0);
	TTF_SetFontKerning(d->font, 0);
	TTF_SetFontHinting(d->font, 0);

	d->surface      = RenderText(d->font, "1234512345");
	d->font_texture = SDL_CreateTextureFromSurface(d->render, d->surface);
}

static void init_display(display_t *display)
{
	unsigned int width  = DISPLAY_WIDTH + DEBUG_REGISTER_WIDTH + DEBUG_INSTRUCTION_WIDTH;
	unsigned int height = DISPLAY_HEIGHT;
	display->window = SDL_CreateWindow("Window", 0, 0, PIXEL_SCALE * width,PIXEL_SCALE * height, 0);

	SDL_Error(display->window == NULL);

	display->render  = SDL_CreateRenderer(display->window, -1, 0);
	SDL_Error(display->render == NULL);

	display->texture = SDL_CreateTexture(display->render,
                                         SDL_PIXELFORMAT_RGB888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         width, height);

	SDL_Error(display->texture == NULL);

	SDL_Error(
		SDL_SetRenderDrawColor(display->render, 0xff, 0xff, 0xff, 0xff) < 0);

	if(REGISTER_WINDOW)
	{
		init_ttf(display);
	}
}

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

display_t *display_init(cpu_state_t *state)
{
	display_t *display = malloc(sizeof(display_t));
	display->state = state;
	display->mem = state->memory;

    PIXEL_SCALE = cmdline_args.scale;

    SDL_Init(SDL_INIT_VIDEO);
    pthread_t thread;
    pthread_create(&thread, NULL, display_thread, display);
	return display;
}


void display_delete(display_t *disp)
{
	SDL_DestroyWindow(disp->window);
	free(disp);
}


static void transfer_buffer(display_t *d)
{
	uint8_t *data;
	int      pitch;
	const SDL_Rect rect = {.x = 0, .y = 0,.w = DISPLAY_WIDTH, .h = DISPLAY_HEIGHT};
	SDL_LockTexture(d->texture, &rect, (void*)&data, &pitch);
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
	if(DEBUG)
	{
		draw_debug(disp);
	}
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

	disp->surface = RenderText(disp->font, buf);
	disp->font_texture = SDL_CreateTextureFromSurface(disp->render, disp->surface);

	SDL_Error(SDL_UpdateTexture(disp->texture, &debug_rect,
							disp->debug_data, PIXEL_SIZE * DEBUG_REGISTER_WIDTH) < 0);

	SDL_Error(SDL_RenderCopy(disp->render, disp->font_texture, NULL, &temp) < 0);

	SDL_DestroyTexture(disp->font_texture);
	SDL_FreeSurface(disp->surface);
}

void draw_instructions(display_t *display)
{
    char buf[100];
	sprintf(buf, "%-35s", last_instruction);
	draw_line(display, buf, 0, 1, DEBUG_INSTRUCTION_WIDTH);
	sprintf(buf, "%-35s", instruction_buffer);
	draw_line(display, buf, 1, 1, DEBUG_INSTRUCTION_WIDTH);
}

#define DRAWLINE(format, ...)\
    do\
    {\
        sprintf(buf, format, ##__VA_ARGS__);\
        draw_line(disp, buf, cur_line++, column, DEBUG_REGISTER_WIDTH);\
    }\
    while(false)

static void draw_debug(display_t *disp)
{
    if(DEBUG_WINDOW)
    {
        char buf[1024];
        int cur_line = 0;
        int column   = 0;
        DRAWLINE("REGISTERS:    ");
        DRAWLINE("PC    = 0x%04x", disp->state->pc);
        DRAWLINE("AF    = 0x%04x", disp->state->af);
        DRAWLINE("BC    = 0x%04x", disp->state->bc);
        DRAWLINE("DE    = 0x%04x", disp->state->de);
        DRAWLINE("HL    = 0x%04x", disp->state->hl);
        DRAWLINE("SP    = 0x%04x", disp->state->sp);
        DRAWLINE("SCX   = 0x%04x", disp->state->memory->scx);
        DRAWLINE("SCY   = 0x%04x", disp->state->memory->scy);
        DRAWLINE("LY    = 0x%04x", disp->state->memory->ly);
        DRAWLINE("LXC   = 0x%04x", disp->state->memory->lyc);
        DRAWLINE("WX    = 0x%04x", disp->state->memory->wx);
        DRAWLINE("WY    = 0x%04x", disp->state->memory->wy);
        DRAWLINE("Bank  = 0x%04x", disp->state->memory->current_bank);
        DRAWLINE(" ");
        DRAWLINE("LCDC:          ");
        DRAWLINE("BG Display : %u", disp->state->memory->lcdc.bg_display);
        DRAWLINE("OBJ Enable : %u", disp->state->memory->lcdc.obj_enable);
        DRAWLINE("OBJ Size   : %u", disp->state->memory->lcdc.obj_size);
        DRAWLINE("Map Select : %u", disp->state->memory->lcdc.map_select);
        DRAWLINE("Tile Select: %u", disp->state->memory->lcdc.tile_data_select);
        DRAWLINE("Window Disp: %u", disp->state->memory->lcdc.window_display);
        DRAWLINE("Window Map : %u", disp->state->memory->lcdc.window_map);
        DRAWLINE("Enabled    : %u", disp->state->memory->lcdc.enabled);

        column   = 1;
        cur_line = 2;
        DRAWLINE("Interrupt Flags (val/en):");
        DRAWLINE("IME      : %u  ", disp->state->memory->IME);
        DRAWLINE("VBLANK   : %u/%u",
                            disp->state->memory->interrupt.v_blank,
                            disp->state->memory->enabled.v_blank);
        DRAWLINE("LCD STAT : %u/%u",
                            disp->state->memory->interrupt.lcd_status,
                            disp->state->memory->enabled.lcd_status);
        DRAWLINE("TIMER    : %u/%u",
                            disp->state->memory->interrupt.timer,
                            disp->state->memory->enabled.timer);
        DRAWLINE("SERIAL   : %u/%u",
                            disp->state->memory->interrupt.serial,
                            disp->state->memory->enabled.serial);
        DRAWLINE("JOYPAD   : %u/%u",
                            disp->state->memory->interrupt.joypad,
                            disp->state->memory->enabled.joypad);

        DRAWLINE(" ");
        DRAWLINE("TIME REGISTERS ");
        DRAWLINE("TIMA     : %03u", disp->state->memory->tima);
        DRAWLINE("TMA      : %03u", disp->state->memory->tma);
        DRAWLINE("CLK SEL  :   %01u", disp->state->memory->tac.clock_select);
        DRAWLINE("TAC EN   :   %01u", disp->state->memory->tac.enable);
        DRAWLINE(" ");
        DRAWLINE("CONT     :  %03d", disp->state->cont);

        draw_instructions(disp);
    }
}

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

static uint8_t shade_table[4] =
{
	255,
	127,
	63,
	0
};

//TODO: Properly comment this.
#define GET_SHADE(n, x) shade_table[((x >> (2 * n)) & 0x3)]

static uint8_t last_background;
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
    last_background = data[x];
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
