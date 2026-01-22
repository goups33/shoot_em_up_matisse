#include "gestionevent.h"
#include "gestionballe.h"

bool gestionevent::UpdateEvents(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_EVENT_QUIT:
		return false;

	case SDL_EVENT_KEY_DOWN:
		switch (event->key.key)
		{
		case SDLK_ESCAPE:
			return false;
		case SDLK_SPACE:
			shoot = true;
			break;

			// Z pour attaque spéciale (tire vers le haut)
		case SDLK_Z:
			attackUp = true;
			break;

			// Touches pour les directions de tir
		case SDLK_D:
			shootDirection = 0; // Droite
			break;
		case SDLK_Q:
			shootDirection = 2; // Gauche
			break;
		case SDLK_S:
			shootDirection = 3; // Bas
			break;

		case SDLK_RIGHT:
			go_right = true;
			break;
		case SDLK_UP:
			go_up = true;
			break;
		case SDLK_LEFT:
			go_left = true;
			break;
		}

		break;
	case SDL_EVENT_KEY_UP:
		switch (event->key.key)
		{
		case SDLK_SPACE:
			shoot = false;
			break;
		case SDLK_Z:
			attackUp = false;
			break;

		case SDLK_RIGHT:
			go_right = false;
			break;
		case SDLK_UP:
			go_up = false;
			break;
		case SDLK_LEFT:
			go_left = false;
			break;
		case SDLK_DOWN:
			go_down = false;
			break;
		}
		break;
	}

	return true;
}