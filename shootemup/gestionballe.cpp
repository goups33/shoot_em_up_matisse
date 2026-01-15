#include "gestionballe.h"

#include <thread>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int gestionballe::shoobullet(int x,int y, int direction)
{
	Bullet balletemp;
	balletemp.x = x;
	balletemp.y = y;

	float angle = direction * (M_PI / 4.0f); // 45 degrés entre chaque direction
	float speed = 15.0f;

	balletemp.velocityX = cos(angle) * speed;
	balletemp.velocityY = -sin(angle) * speed; // Négatif car Y augmente vers le bas

	gestionbullet.push_back(balletemp);

	return 0;
}

void gestionballe::renderbullet(SDL_Renderer* renderer){
	for (size_t i = 0; i < gestionbullet.size(); i++)
	{
		Bullet const& b = gestionbullet[i];

		SDL_FRect rectangle;
		rectangle.x = b.x;
		rectangle.y = b.y;
		rectangle.h = 5;
		rectangle.w = 10;

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rectangle);
	}

}



void gestionballe::Update_bullet(SDL_Renderer* renderer) {
	//SDL_GetRenderViewport()


		std::vector<size_t> toDelete;

		for (size_t i = 0; i < gestionbullet.size(); i++)
		{
			Bullet& b = gestionbullet[i];

			if (b.x > 1935)
				toDelete.push_back(i);
			b.x = b.x + 10;

			b.x += b.velocityX;
			b.y += b.velocityY;
			
		}
	
		for (auto it = toDelete.rbegin(); it != toDelete.rend(); it++)
		{
			size_t index = *it;
			gestionbullet.erase(gestionbullet.begin() + index);
		}
}

