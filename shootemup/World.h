#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Camera.h"

struct Lantern {
    float x;
    float y;
    float speedFactor;
    float baseY;
    float amplitude;
    float speed_lanterne;
    float time;
};

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
    SDL_Texture* lanternTexture;

    std::vector<Lantern> lanterns;
    void initLanterns();
    void loadLanternSprite(SDL_Renderer* renderer);
    void renderLanterns(SDL_Renderer* renderer, const Camera& camera);
};