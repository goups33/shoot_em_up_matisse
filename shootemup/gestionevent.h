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
	bool attackUp = false;

	int shootDirection = 0; // 0=droite, 1=haut, 2=gauche, 3=bas

	float fireRate = 100.0f;
	float lastShotTime = 0.0F;

	bool UpdateEvents(SDL_Event* event);

	bool canShoot(float currentTime) {
		if (currentTime - lastShotTime >= fireRate) {
			lastShotTime = currentTime;
			return true;
		}
		return false;
	}
};

