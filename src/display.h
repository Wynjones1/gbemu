#pragma once
#include "memory.h"
#include "common.h"

#define DISPLAY_BUFFER_WIDTH  256
#define DISPLAY_BUFFER_HEIGHT 256
#define DISPLAY_MS_PER_LINE   0.111

#if DEBUG_WINDOW
	#define DEBUG_REGISTER_WIDTH     70
	#define DEBUG_INSTRUCTION_WIDTH  70
#else
	#define DEBUG_REGISTER_WIDTH     0
	#define DEBUG_INSTRUCTION_WIDTH  0
#endif

#define LCD_WIDTH  160
#define LCD_HEIGHT 144

#if DISPLAY_SHOW_FULL_FRAMEBUFFER
	#define DISPLAY_WIDTH  256
	#define DISPLAY_HEIGHT 256
#else
	#define DISPLAY_WIDTH  LCD_WIDTH
	#define DISPLAY_HEIGHT LCD_HEIGHT
#endif

typedef struct display   display_t;
typedef struct cpu_state cpu_state_t;

display_t *display_init(cpu_state_t *state);
void       display_delete(display_t *disp);
void       display_draw_pixel(display_t *disp, int x, int y, char *rgb);
void       display_present(display_t *disp);
void       display_clear(display_t *disp);
void       display_display(display_t *display);
void       display_simulate(cpu_state_t *state);
uint8_t    display_get_shade(const uint8_t *tile_data, int i);

extern unsigned char g_video_data[DISPLAY_HEIGHT][DISPLAY_WIDTH];
extern char instruction_buffer[];
extern char last_instruction[];
