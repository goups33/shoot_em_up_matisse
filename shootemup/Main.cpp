#include <SDL3/SDL.h>

#include <algorithm>
#include <fstream>
#include <iostream>

#include "gestionballe.h"
#include "gestionevent.h"
#include "enemi.h"
#include "colision.h"

using namespace std;

int main(int argc, char** argv)
{
	srand(time(0));
	SDL_Window* window = nullptr;
	SDL_Renderer * renderer = nullptr;
	
	gestionballe gestion_b;
	gestionevent gestion_e;
	enemi gestion_enemi;
	colision gest_colision;

	SDL_FRect rectangle;
	rectangle.x = 150;
	rectangle.y = 150;
	rectangle.w = 40;
	rectangle.h = 40;
	


	SDL_SetAppMetadata("SDL Test", "1.0", "games.anakata.test-sdl");
	if (!SDL_Init(SDL_INIT_VIDEO))
		return 1;

	if (!SDL_CreateWindowAndRenderer("LE CUBE QUI BOUGE ", 1920, 1080, SDL_WINDOW_FULLSCREEN, &window, &renderer))
		return 1;

	SDL_SetRenderLogicalPresentation(renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	//pour la v-sync, pas touche sinon jump de frames
	SDL_SetRenderVSync(renderer, 1);

	
	bool keepGoing = true;
	float timePrev = 0;
	while (keepGoing)
	{
		keepGoing = gestion_e.UpdateEvents();
		
		float now = SDL_GetTicks();
		gestion_enemi.time = gestion_enemi.time + now - timePrev;
		float dt = now - timePrev;
		
		if (dt > 0.6) {
			timePrev = now;
			//gestion mouvement
			if (gestion_e.go_left) rectangle.x = rectangle.x - 10;
			if (gestion_e.go_up) rectangle.y = rectangle.y - 10;
			if (gestion_e.go_right) rectangle.x = rectangle.x + 10;
			if (gestion_e.go_down) rectangle.y = rectangle.y + 10;
			
			//gestion balle
			if (gestion_e.shoot) gestion_b.shoobullet(rectangle.x, rectangle.y);
			gestion_b.Update_bullet(renderer);
			 
			//gestion enemi
			if(gestion_enemi.time >=gestion_enemi.spawn_cooldown)gestion_enemi.spawn_enemi();
			gestion_enemi.Update_enemi(renderer);

			//gestion des colisions des balles
			gest_colision.gestion_colision_balle(&gestion_b, &gestion_enemi);

			//bord de l'écran
			rectangle.x = std::clamp(rectangle.x, 0.0f, 1920.0f - rectangle.w);
			rectangle.y = std::clamp(rectangle.y, 0.0f, 1080.0f - rectangle.h);


			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			gestion_b.renderbullet(renderer);
			gestion_enemi.show_enemi(renderer);
			SDL_SetRenderDrawColorFloat(renderer, 255, 0, 0,0);
			SDL_RenderFillRect(renderer, &rectangle);


		}

		SDL_RenderPresent(renderer);


	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}