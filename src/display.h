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

#define LCD_WIDTH      160
#define LCD_HEIGHT     144
#define DISPLAY_WIDTH  LCD_WIDTH
#define DISPLAY_HEIGHT LCD_HEIGHT

#if CONTROLS
#define CONTROLS_HEIGHT 100
#else
#define CONTROLS_HEIGHT 0
#endif

typedef struct display   display_t;
typedef cpu_state_t cpu_state_t;

display_t *display_init(cpu_state_t *state);
void       display_delete(display_t *disp);
void       display_simulate(cpu_state_t *state);
void       display_toggle_fullscreen(display_t *display);
void       display_output_framebuffer(display_t *display, const char *filename);
uint32_t  *display_get_framebuffer(display_t *display);
