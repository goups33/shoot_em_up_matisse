#include "gestionevent.h"
#include "gestionballe.h"

bool gestionevent::UpdateEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			return false;

		case SDL_EVENT_KEY_DOWN:
			switch (event.key.key)
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
				shootDirection = 2; // Haut
				break;
			case SDLK_LEFT:
			case SDLK_Q:
				shootDirection = 4; // Gauche
				break;
			case SDLK_DOWN:
			case SDLK_S:
				shootDirection = 6; // Bas
				break;
				// Diagonales avec les touches numériques du pavé ou combinaisons
			case SDLK_KP_9:
			case SDLK_E:
				shootDirection = 1; // Haut-droite
				break;
			case SDLK_KP_7:
			case SDLK_A:
				shootDirection = 3; // Haut-gauche
				break;
			case SDLK_KP_1:
			case SDLK_W:
				shootDirection = 5; // Bas-gauche
				break;
			case SDLK_KP_3:
			case SDLK_C:
				shootDirection = 7; // Bas-droite
				break;
			}

			break;
		case SDL_EVENT_KEY_UP:
			switch (event.key.key)
			{
			case SDLK_SPACE:
				shoot = false;
				break;
			}
			break;

		}

	}
	
	return true;
}
