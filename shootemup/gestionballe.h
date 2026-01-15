#pragma once
#include <vector>
#include <SDL3/SDL_render.h>
struct Bullet
{
	int x;
	int y;
	int velocityX;
	int velocityY;

};

class gestionballe
{
public:
	std::vector<Bullet> gestionbullet;
	int shoobullet(int x, int y, int direction);
	void Update_bullet(SDL_Renderer* renderer);
	int shoobullet(int x, int y);
	void renderbullet(SDL_Renderer* renderer);
};



