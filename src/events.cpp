#include "events.h"

#define X(new, old) do{\
					mem->dpad.new = down;\
					if(down == 0 && mem->dpad.old == 0)\
						mem->dpad.old = 1;\
					}while(0);

static void key(SDL_Keysym sym, int down, cpu_state_t *state)
{
	memory_t *mem = state->memory;
	static int paused;
	static int step;
	static int frame_limit;
	#if 0
	static int slow;
	#endif
	switch(sym.sym)
	{
		//DPad
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
		//Buttons
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
		//Other
		case SDLK_p:
			if(paused == 0 && down == 0)
			{
				paused = 1;
				state->paused = !state->paused;
			}
			else if(paused == 1 && down == 1)
			{
				paused = 0;
			}
			break;
		case SDLK_e:
			if(step == 0 && down == 0)
			{
				step = 1;
				state->step = !state->step;
			}
			else if(step == 1 && down == 1)
			{
				step = 0;
			}
			break;
		case SDLK_q:
			if(frame_limit == 0 && down == 0)
			{
				frame_limit = 1;
				state->frame_limit = !state->frame_limit;
			}
			else if(frame_limit == 1 && down == 1)
			{
				frame_limit = 0;
			}
			break;
		case SDLK_r:
			state->slow = !down;
			/*
			if(slow == 0 && down == 0)
			{
				slow = 1;
				state->slow = !state->slow;
			}
			else if(slow == 1 && down == 1)
			{
				slow = 0;
			}
			*/
			break;
		case SDLK_k:
			state->store_state = 1;
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
