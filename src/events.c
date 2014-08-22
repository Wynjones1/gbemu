#include "events.h"

static void key(SDL_Keysym sym, int down, cpu_state_t *state)
{
	memory_t *mem = state->memory;
	switch(sym.sym)
	{
		case SDLK_w:
			mem->dpad.up    = down;
			break;
		case SDLK_a:
			mem->dpad.left  = down;
			break;
		case SDLK_d:
			mem->dpad.right = down;
			break;
		case SDLK_s:
			mem->dpad.down  = down;
			break;
		case SDLK_i:
			mem->buttons.a     = down;
			break;
		case SDLK_j:
			mem->buttons.b     = down;
			break;
		case SDLK_f:
			mem->buttons.start = down;
			break;
		case SDLK_g:
			mem->buttons.select = down;
			break;
		case SDLK_ESCAPE:
			exit(0);
			break;
		default:
			break;
	}
	if(down == 0) state->memory->interrupt.joypad = 1;
}

void events_handle(cpu_state_t *state)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				key(event.key.keysym, 0, state);
				break;
			case SDL_KEYUP:
				key(event.key.keysym, 1, state);
				break;
			case SDL_QUIT:
				exit(0);
				break;
			default:
				break;
		}
	}
}
