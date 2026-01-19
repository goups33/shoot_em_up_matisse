#pragma once
#include <vector>
#include <SDL3/SDL_render.h>
#include "Camera.h"
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
	void Update_bullet(SDL_Renderer* renderer, float cameraX, float cameraY, float viewWidth, float viewHeight);
	void renderbullet(SDL_Renderer* renderer);
	void Update_bullet(SDL_Renderer* renderer);
};



