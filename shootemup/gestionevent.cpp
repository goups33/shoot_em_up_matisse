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
			// Touches pour les 8 directions de tir
		case SDLK_RIGHT:
		case SDLK_D:
			shootDirection = 0; // Droite
			break;
		case SDLK_UP:
		case SDLK_Z:
			shootDirection = 1; // Haut
			break;
		case SDLK_LEFT:
		case SDLK_Q:
			shootDirection = 2; // Gauche
			break;
		case SDLK_DOWN:
		case SDLK_S:
			shootDirection = 3; // Bas
			break;
		}

		break;
	case SDL_EVENT_KEY_UP:
		switch (event->key.key)
		{
		case SDLK_SPACE:
			shoot = false;
			break;
		}
		break;

	}
	
	return true;
}
