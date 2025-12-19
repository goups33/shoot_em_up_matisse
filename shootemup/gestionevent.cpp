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
			case SDLK_Q:
				go_left = true;
				break;
			case SDLK_D:
				go_right = true;
				break;
			case SDLK_Z:
				go_up = true;
				break;
			case SDLK_S:
				go_down = true;
				break;
			case SDLK_SPACE:
				shoot = true;
				break;
			}

			break;
		case SDL_EVENT_KEY_UP:
			switch (event.key.key)
			{
			case SDLK_Q:
				go_left = false;
				break;
			case SDLK_D:
				go_right = false;
				break;
			case SDLK_Z:
				go_up = false;
				break;
			case SDLK_S:
				go_down = false;
				break;
			case SDLK_SPACE:
				shoot = false;
				break;
			}
			break;

		}

	}
	
	return true;
}
