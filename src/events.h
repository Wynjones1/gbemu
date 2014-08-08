#include <SDL2/SDL.h>

typedef struct events
{
	int up;
	int down;
	int left;
	int right;
	int start;
	int select ;
	int a;
	int b;
	int quit;
}events_t;

void events_handle(events_t *ev);
