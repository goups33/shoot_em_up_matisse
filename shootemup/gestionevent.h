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

	int shootDirection = 0; // 0=droite, 1=haut-droite, 2=haut, 3=haut-gauche, 4=gauche, 5=bas-gauche, 6=bas, 7=bas-droite


	bool UpdateEvents();
};

