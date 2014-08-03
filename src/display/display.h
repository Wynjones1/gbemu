#pragma once

#define DISPLAY_WIDTH  10
#define DISPLAY_HEIGHT 10

typedef struct display display_t;

display_t *display_init(void);
void display_delete(display_t *disp);
void display_draw_pixel(display_t *disp, int x, int y, char *rgb);
void display_present(display_t *disp);