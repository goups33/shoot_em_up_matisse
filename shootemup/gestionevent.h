#pragma once
#include <SDL3/SDL_events.h>

class gestionevent
{
public:
	bool go_left = false;
	bool go_up = false;
	bool go_right = false;
	bool go_down = false;
	bool shoot = false;

	

	bool UpdateEvents();
};

