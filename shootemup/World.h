#pragma once
#include <SDL3/SDL.h>
#include "Camera.h"

class World {
public:
    explicit World(SDL_Renderer* renderer);
    ~World();

    bool loadBackgrounds(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer, const Camera& camera);
    bool shouldTransition(float playerWorldX) const;

private:
    SDL_Texture* background1;
    SDL_Texture* background2;
};