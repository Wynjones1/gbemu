#include "display.h"
#include "events.h"
#include "cpu.h"
#include "opcodes.h"
#include "debug.h"
#include "logging.h"
#include <stdlib.h>
#include <string.h>
#include "controls_image.h"

static const int PIXEL_SIZE  = 4;
static const int NUMBER_OF_OAM_ELEMENTS = 40;
static int PIXEL_SCALE = 1;

char instruction_buffer[200];
char last_instruction[200];


struct display
{
	SDL_Window   *window;
	SDL_Renderer *render;
	SDL_Texture  *texture;
	cpu_state_t  *state;
	memory_t     *mem;

	unsigned char debug_data[DISPLAY_HEIGHT][DEBUG_REGISTER_WIDTH][4];

	//TTF Data
	SDL_Texture *font_texture;
	SDL_Surface *surface;
    bool         fullscreen;
    bool         cur_buffer;
    uint32_t   (*pixel_buffer)[DISPLAY_WIDTH];
    uint32_t   (*draw_buffer)[DISPLAY_WIDTH];
    uint32_t     buffers[2][DISPLAY_HEIGHT][DISPLAY_WIDTH];

    SDL_cond    *cond;
    SDL_mutex   *lock;
};

#if SDL_TTF
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
#endif

static void init_display(display_t *display)
{
	unsigned int width  = DISPLAY_WIDTH;
	unsigned int height = DISPLAY_HEIGHT;
	display->window = SDL_CreateWindow("Window", 0, 0,
                                       PIXEL_SCALE * width,
                                       PIXEL_SCALE * (height + 100),
                                       SDL_WINDOW_RESIZABLE);

	SDL_Error(display->window == NULL);

	display->render  = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Error(display->render == NULL);
    SDL_Error(SDL_RenderSetLogicalSize(display->render, DISPLAY_WIDTH, DISPLAY_HEIGHT + 100) < 0);
    SDL_Error(SDL_SetRenderDrawColor(display->render, 0, 0, 0, 255) < 0);

	display->texture = SDL_CreateTexture(display->render,
                                         SDL_PIXELFORMAT_RGBA8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         DISPLAY_WIDTH, DISPLAY_HEIGHT + 100);
	SDL_Error(display->texture == NULL);
    SDL_Error(SDL_ShowCursor(SDL_DISABLE) < 0);
}

static int display_thread(void *display_)
{
	display_t *display = display_;
	init_display(display);
	g_state = display->state;
	while(1)
	{
		events_handle(display->state);
        SDL_Delay(17);
        SDL_LockMutex(display->lock);
		display_display(display);
        SDL_UnlockMutex(display->lock);
	}
	return 0;
}

uint32_t fake_buffer[100 * DISPLAY_HEIGHT * 100];
display_t *display_init(cpu_state_t *state)
{
	display_t *display = malloc(sizeof(display_t));
	display->state     = state;
	display->mem       = state->memory;
    display->cond      = SDL_CreateCond();
    display->lock      = SDL_CreateMutex();
    PIXEL_SCALE        = cmdline_args.scale;

    display->pixel_buffer = display->buffers[display->cur_buffer];
    display->draw_buffer  = display->buffers[!display->cur_buffer];

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateThread(display_thread, "Display Thread", display);
    char *temp = header_data;
    for(uint32_t i = 0; i < width * height; i++)
    {
        uint8_t pixel[3];
        HEADER_PIXEL(temp, pixel);
        fake_buffer[i] = pixel[0] << 24 | pixel[1] << 16 | pixel[0] << 8 | 255;
    }
	return display;
}


void display_delete(display_t *disp)
{
	SDL_DestroyWindow(disp->window);
	free(disp);
}


void display_display(display_t *display)
{
	//Display the image.
	if(display->mem->lcdc.enabled)
	{
        SDL_Rect screen   = {0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT};
        SDL_Rect controls = {0, DISPLAY_HEIGHT, DISPLAY_WIDTH, 100};
        SDL_UpdateTexture(display->texture, &screen  , display->draw_buffer, DISPLAY_WIDTH * sizeof(uint32_t));
        SDL_UpdateTexture(display->texture, &controls, fake_buffer, DISPLAY_WIDTH * sizeof(uint32_t));
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

void display_present(display_t *disp)
{
	SDL_Error(SDL_RenderClear(disp->render) < 0);
	SDL_Error(SDL_RenderCopy(disp->render, disp->texture, NULL, NULL) < 0);
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
	if(BIT_N(sprite->flags, X_FLIP_BIT))
	{
		ox = 7 - ox;
	}
	if(BIT_N(sprite->flags,Y_FLIP_BIT))
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
    memory_t *memory = state->memory;
	uint8_t size = memory->lcdc.obj_size ? 16 : 8;
	for(uint8_t i = 0; i < NUMBER_OF_OAM_ELEMENTS; i++)
	{
		sprite = memory->oam_data + memory->oam_index_sort[i];
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
                return out;
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
#define MAKE_PIXEL(x) (x << 24 | x << 16 | x << 8 | 0xff)
#define GET_INDEX(n, x) ((x >> (2 * n)) & 0x3)
#define GET_SHADE(n, x) MAKE_PIXEL(shade_table[GET_INDEX(n, x)])

static uint8_t last_background;
static void write_sprites(struct cpu_state *state, display_t *display, uint8_t x, uint8_t y)
{
	uint32_t *data = display->pixel_buffer[y];
	struct OAM_data *sprite = get_sprite(state, x, y);
	if(sprite)
	{
		uint8_t palette = BIT_N(sprite->flags, PALETTE_BIT) ? state->memory->obp1 : state->memory->obp0;
        if(BIT_N(sprite->flags, PRIORITY_BIT) == 0 || last_background == 0)
            data[x] = GET_SHADE(g_shade, palette);
	}
}

/* Write the current line that is drawing into the framebuffer */
static void write_background(struct cpu_state *state, display_t *display, uint8_t x, uint8_t y)
{
	int ty        = (y + state->memory->scy) / 8;
	int offset    = (y + state->memory->scy) % 8;
	int scx = state->memory->scx;
	uint32_t *data = display->pixel_buffer[y];
	const uint8_t *tile_data;
	int tx = ((x + scx) / 8) % 0x20;
	int ox = (x + scx) % 8;
	tile_data = memory_get_tile_data(state->memory, tx, ty, offset, state->memory->lcdc.map_select);
	data[x] = GET_SHADE(display_get_shade(tile_data, ox), state->memory->bgp);
    last_background = GET_INDEX(display_get_shade(tile_data, ox), state->memory->bgp);
}

static void write_window(struct cpu_state *state, display_t *display, uint8_t x, uint8_t y)
{
	int wx = state->memory->wx - 7;
	int wy = state->memory->wy;
	if(state->memory->lcdc.window_display && y > wy && wx <= x)
	{
		uint32_t *data = display->pixel_buffer[y];
		int tx = (x - wx) / 8;
		int ox = (x - wx) % 8;
		int ty = (y - wy) / 8;
		int oy = (y - wy) % 8;
		const uint8_t *tile_data = memory_get_tile_data(state->memory, tx, ty, oy, state->memory->lcdc.window_map);
		data[x] = GET_SHADE(display_get_shade(tile_data, ox), state->memory->bgp);
	}
}

static void write_display(struct cpu_state *state, display_t *display)
{
	if(state->memory->ly < DISPLAY_HEIGHT)
	{
		for(int i = 0; i < DISPLAY_WIDTH; i++)
		{
            uint8_t y = state->memory->ly;
			write_background(state, display, i, y);
			write_window(state, display, i, y);
			write_sprites(state, display, i, y);
		}
	}
}

void display_simulate(struct cpu_state *state)
{
	if(state->clock_counter >= CPU_CLOCKS_PER_LINE)
	{
		state->clock_counter -= CPU_CLOCKS_PER_LINE;
		state->memory->ly = (state->memory->ly + 1) % 154;
		if(state->memory->ly == state->memory->lyc)
		{
			state->memory->stat.coincidence = 1;
			if(state->memory->stat.coincidence_int)
			{
                SET_N(state->memory->IF, LCD_STATUS_BIT);
			}
		}
		else
		{
			state->memory->stat.coincidence = 0;
		}

		if(state->memory->ly == 144)
		{
            SET_N(state->memory->IF, VBLANK_BIT);
            display_draw(state->display);
		}
		write_display(state, state->display);
	}
}

void display_toggle_fullscreen(display_t *display)
{
    display->fullscreen = !display->fullscreen;
    if(display->fullscreen)
    {
        SDL_SetWindowFullscreen(display->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        SDL_SetWindowFullscreen(display->window, 0);
    }
}

void display_draw(display_t *display)
{
    display->cur_buffer   = !display->cur_buffer;
    display->pixel_buffer = display->buffers[display->cur_buffer];
    display->draw_buffer  = display->buffers[!display->cur_buffer];
    SDL_LockMutex(display->lock);
    SDL_CondSignal(display->cond);
    SDL_UnlockMutex(display->lock);
}
