#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Camera.h"
#include "Player.h"
#include "World.h"
#include "gestionballe.h"
#include "gestionevent.h"
#include "enemi.h"
#include "colision.h"
#include "Button.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Game {
public:
    Game(SDL_Renderer* renderer, TTF_Font* font);
    ~Game();

    void handleEvent(SDL_Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    bool isShowingTransition() const { return showWorldTransition; }
    bool shouldReturnToMenu() const { return returnToMenu; }
    void resetReturnToMenu() { returnToMenu = false; }

private:
    Camera camera;
    Player* player;
    World* world;
    gestionballe gestion_b;
    gestionevent gestion_e;
    enemi gestion_enemi;
    colision gest_colision;

    TTF_Font* font;
    Button buttonContinue;
    Button buttonQuitGame;

    bool showWorldTransition;
    bool returnToMenu;
    float timePrev;

    SDL_Renderer* renderer;
    SDL_Texture* bulletTexture;
    SDL_Texture* enemyTexture;

    void renderTransition(SDL_Renderer* renderer);
    void renderGame(SDL_Renderer* renderer);
    void loadBulletSprite();
    void loadEnemySprite();
};