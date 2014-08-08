#pragma once
#include "memory.h"

#define DISPLAY_WIDTH  256
#define DISPLAY_HEIGHT 256

typedef struct display display_t;

display_t *display_init(memory_t *mem);
void display_delete(display_t *disp);
void display_draw_pixel(display_t *disp, int x, int y, char *rgb);
void display_present(display_t *disp);
