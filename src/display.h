#pragma once
#include "memory.h"

#define LCD_WIDTH  160
#define LCD_HEIGHT 144

#if DISPLAY_SHOW_FULL_FRAMEBUFFER
	#define DISPLAY_WIDTH   256
	#define DISPLAY_HEIGHT  256
#else
	#define DISPLAY_WIDTH  LCD_WIDTH
	#define DISPLAY_HEIGHT LCD_HEIGHT
#endif

#define DISPLAY_BUFFER_WIDTH  256
#define DISPLAY_BUFFER_HEIGHT 256

typedef struct display display_t;

display_t *display_init(memory_t *mem);
void display_delete(display_t *disp);
void display_draw_pixel(display_t *disp, int x, int y, char *rgb);
void display_present(display_t *disp);
void display_clear(display_t *disp);
