#pragma once
#include <vector>
#include <SDL3/SDL_render.h>
struct Bullet
{
	int x;
	int y;

};

class gestionballe
{
public:
	std::vector<Bullet> gestionbullet;
	int shoobullet(int x, int y);
	void Update_bullet(SDL_Renderer* renderer);
	void renderbullet(SDL_Renderer* renderer);
};



