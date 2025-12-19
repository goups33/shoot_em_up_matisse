#include "enemi.h"
#include <random>

void enemi::spawn_enemi() {
	int y = rand() %  1000 + 50;
	time = 0.0;
	mechan enemitemp;
	enemitemp.x = 1900;
	enemitemp.y = y;
	gestion_enemi.push_back(enemitemp);
}

//SDL_HasRectIntersection

void enemi::show_enemi(SDL_Renderer* renderer) {
	for (size_t i = 0; i < gestion_enemi.size(); i++)
	{
		mechan& b = gestion_enemi[i];

		SDL_FRect rectangle;
		rectangle.x = b.x;
		rectangle.y = b.y;
		rectangle.h = 20;
		rectangle.w = 20;

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rectangle);
	}
}

void enemi::Update_enemi(SDL_Renderer* renderer) {
	//SDL_GetRenderViewport()


	std::vector<size_t> toDelete;

	//for (Bullet& b : gestionbullet) {
	for (size_t i = 0; i < gestion_enemi.size(); i++)
	{
		mechan& b = gestion_enemi[i];

		if (b.x < 0)
			toDelete.push_back(i);
		b.x = b.x -10 ;

	}

	for (auto it = toDelete.rbegin(); it != toDelete.rend(); it++)
	{
		size_t index = *it;
		gestion_enemi.erase(gestion_enemi.begin() + index);
	}
}