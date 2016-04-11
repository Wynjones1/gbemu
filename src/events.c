#include "events.h"
#include "logging.h"
#include "SDL.h"
#define X(new, old) do{\
					mem->dpad.new = down;\
					if(down == 0 && mem->dpad.old == 0)\
						mem->dpad.old = 1;\
					}while(0);

#if 0
#define LogInput(format)\
    log_message(format ": %s", down ? "up" : "down")
#else
#define LogInput(format, ...)
#endif
static void key(SDL_Keysym sym, int down, cpu_state_t *state)
{
	memory_t *mem = state->memory;
	static int paused;
	static int step;
	static int frame_limit;
	static int slow;
	switch(sym.sym)
	{
		//DPad
		case SDLK_w:
            LogInput("Up");
			X(up, down);
			break;
		case SDLK_a:
            LogInput("Left");
			X(left, right);
			break;
		case SDLK_d:
            LogInput("Right");
			X(right, left);
			break;
		case SDLK_s:
            LogInput("Down");
			X(down, up);
			break;
		//Buttons
		case SDLK_i:
            LogInput("A");
			mem->buttons.a     = down;
			break;
		case SDLK_j:
            LogInput("B");
			mem->buttons.b     = down;
			break;
		case SDLK_f:
            LogInput("Select");
			mem->buttons.start = down;
			break;
		case SDLK_g:
            LogInput("Start");
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
			SDL_PauseAudio(state->paused);
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
			break;
        case SDLK_x:
            if(down == 1)
            {
                state->cont   = 1;
                state->paused = 0;
            }
            break;
		case SDLK_F5:
			state->store_state = 1;
			break;
        case SDLK_F6:
            state->load_state = 1;
            break;
        case SDLK_F11:

            if(down)
            {
                display_toggle_fullscreen(state->display);
            }
            break;
		case SDLK_ESCAPE:
            state->exit = 1;
			break;
		default:
			break;
	}
	if(down == 0) SET_N(state->memory->IF, JOYPAD_BIT);
}
#undef X

int handle_events(cpu_state_t *state)
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
            case SDL_FINGERDOWN:
                event.type = SDL_KEYDOWN;
                event.key.keysym.sym = SDLK_f;
                SDL_PushEvent(&event);
                log_message("FingerDown   %f %f", event.tfinger.x, event.tfinger.y);
                break;
            case SDL_FINGERUP:
                event.type = SDL_KEYUP;
                event.key.keysym.sym = SDLK_f;
                SDL_PushEvent(&event);
                log_message("FingerUp   %f %f", event.tfinger.x, event.tfinger.y);
                break;
			case SDL_QUIT:
                state->exit = 1;
				break;
			default:
				break;
		}
	}
    return 0;
}
