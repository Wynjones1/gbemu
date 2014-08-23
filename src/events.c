#include "events.h"

#define X(new, old) do{\
					mem->dpad.new = down;\
					if(down == 0 && mem->dpad.old == 0)\
						mem->dpad.old = 1;\
					}while(0);

static void key(SDL_Keysym sym, int down, cpu_state_t *state)
{
	memory_t *mem = state->memory;
	switch(sym.sym)
	{
		case SDLK_w:
			X(up, down);
			break;
		case SDLK_a:
			X(left, right);
			break;
		case SDLK_d:
			X(right, left);
			break;
		case SDLK_s:
			X(down, up);
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
#undef X

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
