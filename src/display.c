#include "display.h"
#include "ppm.h"
#include "events.h"
#include "cpu.h"
#include "opcodes.h"
#include "debug.h"
#include "logging.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "controls_image.h"
#include "embedded_font.h"
#include "instruction_strings.h"

static const int PIXEL_SIZE  = 4;
static const int NUMBER_OF_OAM_ELEMENTS = 40;
static int PIXEL_SCALE = 1;

static uint8_t get_shade(const uint8_t *tile_data, int i);

#if CONTROLS
    uint32_t control_buffer[DISPLAY_WIDTH * CONTROLS_HEIGHT];
#endif 
uint32_t white_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

typedef struct
{
#if HAVE_TTF
    uint32_t width, height;
    uint32_t font_width, font_height;
    uint32_t x, y;
    SDL_Color     fg;
    TTF_Font     *font;
    SDL_Renderer *renderer;
    SDL_Surface  *glyphs[128 - 20];
    SDL_Texture  *glyph_textures[128 - 20];
    SDL_Rect      rect;
    char          lines[];
#else
    uint32_t reserved;
#endif
}text_area_t;

struct display
{
	SDL_Window   *window;
	SDL_Renderer *render;
	SDL_Texture  *texture;
	cpu_state_t  *state;
	memory_t     *mem;
    text_area_t  *text_area;

	SDL_Surface *surface;
    bool         cur_buffer;
    bool         fullscreen;
    uint32_t   (*pixel_buffer)[DISPLAY_WIDTH];
    uint32_t   (*draw_buffer)[DISPLAY_WIDTH];
    uint32_t     buffers[2][DISPLAY_HEIGHT][DISPLAY_WIDTH];
};

char *text_area_get_line(text_area_t *ta, uint32_t line);

SDL_Rect make_rect(uint32_t xmin, uint32_t ymin, uint32_t width, uint32_t height)
{
    SDL_Rect out;
    out.x  = xmin;
    out.y  = ymin;
    out.w  = width;
    out.h  = height;
    return out;
}

text_area_t *text_area_init(SDL_Renderer *render, TTF_Font *font,
                            uint32_t width, uint32_t height, uint32_t x, uint32_t y)
{
#if HAVE_TTF
    text_area_t *out = (text_area_t*) calloc(1, sizeof(text_area_t) + (width + 1) * height);
    out->width       = width;
    out->height      = height;
    out->renderer    = render;
    out->fg          = (SDL_Color){255, 255, 255, 255};
    out->font        = font;
    out->x           = x;
    out->y           = y;
    int xx, yy;
    TTF_SizeText(out->font, "A", &xx, &yy);
    out->rect        = make_rect(x, y, xx * width, yy * height);
    out->font_width  = xx;
    out->font_height = yy;
    for(uint8_t i = 20; i < 128; i++)
    {
        out->glyphs[i - 20]         = TTF_RenderGlyph_Blended(font, i, out->fg);
        out->glyph_textures[i - 20] = SDL_CreateTextureFromSurface(out->renderer, out->glyphs[i - 20]);
    }
    for(uint32_t i = 0; i < height; i++)
    {
        memset(text_area_get_line(out, i), ' ', width);
    }
    return out;
#else
    return NULL;
#endif
}

char *text_area_get_line(text_area_t *ta, uint32_t line)
{
#if HAVE_TTF
    if(line >= ta->height) return NULL;
    return &ta->lines[(ta->width + 1) * line];
#else
    return NULL;
#endif
}

void text_area_printf(text_area_t *ta, int lineno, const char *format, ...)
{
#if HAVE_TTF
    va_list ap;
    va_start(ap, format);
    char *line = text_area_get_line(ta, lineno);
    vsnprintf(line, (ta->width + 1), format, ap);
    line[ta->width] = '\0';
    va_end(ap);
#endif
}

void text_area_draw(text_area_t *ta)
{
#if HAVE_TTF
    for(uint32_t i = 0; i < ta->height; i++)
    {
        char *line = text_area_get_line(ta, i);
        bool end = false;
        for(uint32_t j = 0; j < ta->width; j++)
        {
            SDL_Rect dest = make_rect(ta->x + (ta->font_width  * j),
                             ta->y + (ta->font_height * i),
                             ta->font_width,
                             ta->font_height);
            SDL_Texture *tex;
            if(!end)
            {
                if(line[j] == '\0')
                    end = true;
                else
                    tex = ta->glyph_textures[line[j] - 20];
            }

            if(end)
            {
                tex = ta->glyph_textures[' ' - 20];
            }
            SDL_RenderCopy(ta->renderer, tex, NULL, &dest);
        }
    }
#endif
}

SDL_RWops *read_font(void)
{
#if 1
    return SDL_RWFromMem(droid_font_array, droid_font_size);
#else
    return SDL_RWFromFile("./data/fonts/DroidSansMono.ttf", "rb");
#endif
}


static uint8_t get_shade(const uint8_t *tile_data, int i)
{
	return ((tile_data[1] >> (7 - (i))) & 0x1) << 1 | ((tile_data[0] >> (7 - (i))) & 0x1);
}

static void write_framebuffer(display_t *display)
{
    SDL_Error(SDL_RenderClear(display->render) < 0);

    // Write the framebuffer onto the the texture.
    SDL_Rect screen   = {0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT};
	if(display->mem->lcdc.enabled)
	{
        SDL_UpdateTexture(display->texture, &screen  , display->draw_buffer, DISPLAY_WIDTH * sizeof(uint32_t));
	}
    else
    {
        SDL_UpdateTexture(display->texture, &screen, white_buffer, DISPLAY_WIDTH * sizeof(uint32_t));
    }

#if CONTROLS
    SDL_Rect controls = {0, DISPLAY_HEIGHT, DISPLAY_WIDTH, CONTROLS_HEIGHT};
    SDL_UpdateTexture(display->texture, &controls, control_buffer, DISPLAY_WIDTH * sizeof(uint32_t));
#endif

    // Draw the framebuffer texture onto the screen.
    SDL_Rect output = {0, 0, PIXEL_SCALE * DISPLAY_WIDTH, PIXEL_SCALE * (DISPLAY_HEIGHT + CONTROLS_HEIGHT)};
    SDL_Error(SDL_RenderCopy(display->render, display->texture, NULL, &output) < 0);

#if DEBUG_WINDOW
    text_area_draw(display->text_area);
#endif

    SDL_RenderPresent(display->render);
}

static void draw_debug(display_t *display)
{
#if DEBUG_WINDOW
    text_area_t *ta = display->text_area;
    cpu_state_t *state = display->state;
    memory_t *memory = display->state->memory;
    uint32_t line = 0;
    uint32_t bank = display->state->memory->current_bank;
    
    #define X(n) ((state->f >> n ## _BIT) & 0x1)
    text_area_printf(ta, line++, "+-------------------------+");
    text_area_printf(ta, line++, "|        Registers:       |");
    text_area_printf(ta, line++, "| AF = 0x%04x BC = 0x%04x |", state->af, state->bc);
    text_area_printf(ta, line++, "| DE = 0x%04x HL = 0x%04x |", state->de, state->hl);
    text_area_printf(ta, line++, "| SP = 0x%04x PC = 0x%04x |", state->sp, state->pc);
    text_area_printf(ta, line++, "| C=%d   HC=%d  Z=%d  S=%d    |", X(CARRY), X(HALF_CARRY), X(ZERO), X(SUBTRACT));
    text_area_printf(ta, line++, "+-------------------------+");
    text_area_printf(ta, line++, "| LCDC    = 0x%02x          |", *(uint8_t*)&memory->lcdc);
    text_area_printf(ta, line++, "| STAT    = 0x%02x          |", *(uint8_t*)&memory->stat);
    text_area_printf(ta, line++, "| DIV     = 0x%02x          |", *(uint8_t*)&memory->div);
    text_area_printf(ta, line++, "| TIMA    = 0x%02x          |", *(uint8_t*)&memory->tima);
    text_area_printf(ta, line++, "| TMA     = 0x%02x          |", *(uint8_t*)&memory->tma);
    text_area_printf(ta, line++, "| TAC     = 0x%02x          |", *(uint8_t*)&memory->tac);
    text_area_printf(ta, line++, "| IME     = 0x%02x          |", *(uint8_t*)&memory->IME);
    text_area_printf(ta, line++, "| IE  = 0x%02x IF  = 0x%02x   |", *(uint8_t*)&memory->IE, *(uint8_t*)&memory->IF);
    text_area_printf(ta, line++, "| LY  = 0x%02x LYC = 0x%02x   |", *(uint8_t*)&memory->ly, *(uint8_t*)&memory->lyc);
    text_area_printf(ta, line++, "| DPD = 0x%02x BTN = 0x%02x   |", *(uint8_t*)&memory->dpad, *(uint8_t*)&memory->buttons);
    text_area_printf(ta, line++, "| SCX = 0x%02x SCY = 0x%02x   |", memory->scx, memory->scy);
	text_area_printf(ta, line++, "|  WX = 0x%02x  WY = 0x%02x   |", memory->wx, memory->wy);
    text_area_printf(ta, line++, "+-------------------------+");
    text_area_printf(ta, line++, "|  %6s   | %11s |", state->paused ? "paused" : "", state->frame_limit ? "frame limit" : "");
    text_area_printf(ta, line++, "+-------------------------+");
    text_area_printf(ta, line++, "|         %6.2f%%         |", state->fps);
    text_area_printf(ta, line++, "+-------------------------+");
    text_area_printf(ta, line++, "|     (HL) = 0x%04x       |", memory_load16(state->memory, state->hl));
    text_area_printf(ta, line++, "|      arg = 0x%04x       |", memory_load16(state->memory, state->pc + 1));
    text_area_printf(ta, line++, "|    stack = 0x%04x       |", memory_load16(state->memory, state->sp));
    text_area_printf(ta, line++, "+-------------------------+");

    reg16_t pc = state->pc;
    reg16_t addr = pc;
    for(uint32_t i = line; i < ta->height; i++)
    {
        reg_t  inst = memory_load8(state->memory, addr);
        const char *inst_string = instruction_strings[inst];
        if(addr > 0x3fff)
            bank = 0;
        if(inst == 0xCB)
        {
            inst = memory_load8(state->memory, addr);
            addr += 1;
            inst_string = instruction_strings_cb[inst];
        }
        text_area_printf(ta, i, "%02x:0x%04x: %02x %s", bank, addr, inst, inst_string);
        addr += op_table[inst].size;
    }
#endif
}

static void display_init_(display_t *display)
{
    SDL_Init(SDL_INIT_VIDEO);
    int fwidth  = 0;
    int fheight = 0;
#if HAVE_TTF
    TTF_Init();
    TTF_Font  *font = NULL;
    SDL_RWops *font_src = read_font();
    TTF_Error((font = TTF_OpenFontRW(font_src, 0, 15)) == NULL);
    TTF_SizeText(font, "A", &fwidth, &fheight);
#endif
#if DEBUG_WINDOW
    uint32_t text_width  = 27;
    uint32_t text_height = 40;
#else
    uint32_t text_width  = 0;
    uint32_t text_height = 0;
#endif

	uint32_t width  = DISPLAY_WIDTH ;
	uint32_t height = DISPLAY_HEIGHT + CONTROLS_HEIGHT;
	display->window = SDL_CreateWindow("GBemu", 30, 30,
                                       PIXEL_SCALE * width + (text_width * fwidth),
                                       max(PIXEL_SCALE * height, text_height * fheight),
                                       SDL_WINDOW_RESIZABLE);

	SDL_Error(display->window == NULL);

	display->render = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Error(display->render == NULL);
    SDL_Error(SDL_RenderSetLogicalSize(display->render,
                                       PIXEL_SCALE * width + (text_width * fwidth),
                                       max(PIXEL_SCALE * height, text_height * fheight)) < 0);
    SDL_Error(SDL_SetRenderDrawColor(display->render, 0, 0, 0, 255) < 0);

	display->texture = SDL_CreateTexture(display->render,
                                         SDL_PIXELFORMAT_RGBA8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         width, height);
	SDL_Error(display->texture == NULL);
    SDL_Error(SDL_ShowCursor(SDL_DISABLE) < 0);

#if DEBUG_WINDOW
    display->text_area  = text_area_init(display->render, font, text_width, text_height, PIXEL_SCALE * DISPLAY_WIDTH, 0);
#endif
    display->fullscreen = false;
}

static int display_thread(void *display_)
{
	display_t *display = (display_t*) display_;
	display_init_(display);
	g_state = display->state;
	while(1)
	{
        SDL_Delay(17);
        draw_debug(display);
        write_framebuffer(display);
        handle_events(display->state);
	}
	return 0;
}

display_t *display_init(cpu_state_t *state)
{
	display_t *display = (display_t*)calloc(1, sizeof(display_t));
	display->state     = state;
	display->mem       = state->memory;
    PIXEL_SCALE        = cmdline_args.scale;

    display->pixel_buffer = display->buffers[display->cur_buffer];
    display->draw_buffer  = display->buffers[!display->cur_buffer];

    SDL_CreateThread(display_thread, "Display Thread", display);
    char *temp = header_data;
    memset(white_buffer, 0xff, sizeof(white_buffer));
#if CONTROLS
    for(uint32_t i = 0; i < width * height; i++)
    {
        uint8_t pixel[3];
        HEADER_PIXEL(temp, pixel);
        control_buffer[i] = pixel[0] << 24 | pixel[1] << 16 | pixel[0] << 8 | 255;
    }
#endif

	return display;
}

void display_delete(display_t *disp)
{
	SDL_DestroyWindow(disp->window);
	free(disp);
}

static int get_sprite_shade(cpu_state_t *state, struct OAM_data *sprite, int x, int y)
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
	return get_shade(tile_data, ox);
}

int g_shade;
static struct OAM_data *get_sprite(cpu_state_t *state, int x, int y)
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

static void signal_draw_thread(display_t *display)
{
    display->cur_buffer   = !display->cur_buffer;
    display->pixel_buffer = display->buffers[display->cur_buffer];
    display->draw_buffer  = display->buffers[!display->cur_buffer];
}

static void set_mode(cpu_state_t *state)
{
    if(state->memory->ly >= 144)
    {
        state->memory->stat.mode = 0x1;
    }
    else
    {
        state->memory->stat.mode = 0x0;
    }
}

void display_toggle_fullscreen(display_t *display)
{
    display->fullscreen = !display->fullscreen;
    uint32_t flags = display->fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    SDL_SetWindowFullscreen(display->window, flags);
}

void display_output_framebuffer(display_t *display, const char *filename)
{
	ppm_t *ppm = ppm_new(256, 256, filename);
    cpu_state_t *state = display->state;
	for(int i = 0; i < 32; i++)
	{
		for(int j = 0; j < 32; j++)
		{
			for(int o = 0; o < 8; o++)
			{
				const uint8_t *data = memory_get_tile_data(state->memory, i, j, o, state->memory->lcdc.map_select);
				for(int k = 0; k < 8;k++)
				{
					uint8_t shade = shade_table[get_shade(data, k)];
					uint8_t pixel[] = {shade, shade, shade};
					ppm_write_pixel(ppm,  8 * i + k, 8 * j + o, pixel);
				}
			}
		}
	}
}

//TODO: Properly comment this.
#define MAKE_PIXEL(x) (x << 24 | x << 16 | x << 8 | 0xff)
#define GET_INDEX(n, x) ((x >> (2 * n)) & 0x3)
#define GET_SHADE(n, x) MAKE_PIXEL(shade_table[GET_INDEX(n, x)])

static uint8_t last_background;
static void write_sprites(cpu_state_t *state, display_t *display, uint8_t x, uint8_t y)
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
static void write_background(cpu_state_t *state, display_t *display, uint8_t x, uint8_t y)
{
	const uint8_t *tile_data;
	uint8_t scx = state->memory->scx;
	uint8_t scy = state->memory->scy;
	uint32_t *data = display->pixel_buffer[y];

	//Calculate the tile x, y and offset in the tile.
	int tx = ((x + scx) / 8) % 0x20;
	int ty = ((y + scy) / 8) % 0x20;
	int ox = (x + scx) % 8;
	int oy = (y + scy) % 8;

	tile_data = memory_get_tile_data(state->memory, tx, ty, oy, state->memory->lcdc.map_select);
	data[x] = GET_SHADE(get_shade(tile_data, ox), state->memory->bgp);
    last_background = GET_INDEX(get_shade(tile_data, ox), state->memory->bgp);
}

static void write_window(cpu_state_t *state, display_t *display, uint8_t x, uint8_t y)
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
		data[x] = GET_SHADE(get_shade(tile_data, ox), state->memory->bgp);
	}
}

static void write_display(cpu_state_t *state, display_t *display)
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

void display_simulate(cpu_state_t *state)
{
	if(state->clock_counter >= CPU_CLOCKS_PER_LINE)
	{
		state->clock_counter -= (uint32_t)CPU_CLOCKS_PER_LINE;
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
            signal_draw_thread(state->display);
		}
        set_mode(state);
		write_display(state, state->display);
	}
}
