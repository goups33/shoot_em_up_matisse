#include "colision.h"

void colision::gestion_colision_balle(gestionballe* balle, enemi* malotru) {
	std::vector <int> delete_mechan;
	std::vector <int> delete_balle;
	for (size_t i = 0; i < malotru->gestion_enemi.size(); i++) {
		SDL_Rect e;
		e.x = malotru->gestion_enemi[i].x;
		e.y = malotru->gestion_enemi[i].y;
		e.w = 20;
		e.h = 20;

		for (size_t j = 0; j < balle->gestionbullet.size(); j++) {
			SDL_Rect b;
			b.x = balle->gestionbullet[j].x;
			b.y =balle->gestionbullet[j].y;
			b.w = 5;
			b.h = 10;
			
			if (SDL_HasRectIntersection(&e, &b)) {
				delete_balle.push_back(j);
				delete_mechan.push_back(i);
				printf("POUET");
			}
		}
	}
	for (auto it = delete_balle.rbegin(); it != delete_balle.rend(); it++)
	{
		size_t index = *it;
		balle->gestionbullet.erase(balle->gestionbullet.begin() + index);
	}

	for (auto it = delete_mechan.rbegin(); it != delete_mechan.rend(); it++)
	{
		size_t index = *it;
		malotru->gestion_enemi.erase(malotru->gestion_enemi.begin() + index);
	}
}