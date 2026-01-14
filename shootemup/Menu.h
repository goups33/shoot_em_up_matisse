#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Button.h"
#include "GameState.h"


class Menu
{

public:
    Menu(SDL_Window* window, TTF_Font* font, SDL_Renderer* renderer);
    ~Menu();
    void handleEvent(const SDL_Event& event, GameState& currentState);
    void draw(SDL_Renderer* renderer);

private:
    TTF_Font* font;
    int windowWidth;
    int windowHeight;

    Button buttonPlay;
    Button buttonQuit;

    SDL_Texture* backgroundTexture;
    bool loadBackground(SDL_Renderer* renderer, const char* imagePath);
};

