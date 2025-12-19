#pragma once
#include <vector>
#include <SDL3/SDL_render.h>


struct mechan {
	int x;
	int y;
};

class enemi
{
public:
	int pos_x;
	int pos_y;
	float time = 0.0;
	float spawn_cooldown = 1000.0;
	std::vector<mechan> gestion_enemi;

	void show_enemi(SDL_Renderer* renderer);
	void spawn_enemi();
	void Update_enemi(SDL_Renderer* renderer);
};

