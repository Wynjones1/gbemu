#include "events.h"

static void key(events_t *ev, SDL_Keysym sym, int down)
{
	switch(sym.sym)
	{
		case SDLK_w:
			ev->up    = down;
			break;
		case SDLK_a:
			ev->left  = down;
			break;
		case SDLK_d:
			ev->right = down;
			break;
		case SDLK_s:
			ev->down  = down;
			break;
		case SDLK_i:
			ev->a     = down;
			break;
		case SDLK_j:
			ev->b     = down;
			break;
		case SDLK_ESCAPE:
			ev->quit  = down;
			break;
		default:
			break;
	}
}

void events_handle(events_t *ev)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				key(ev, event.key.keysym, 1);
				break;
			case SDL_KEYUP:
				key(ev, event.key.keysym, 0);
				break;
		}
	}
}
