#pragma once
#include <SDL3/SDL.h>
#include "Button.h"

class Select
{
public:
    Select(SDL_Window* window, TTF_Font* font);
    void handleEvent(const SDL_Event& event, int& selectedLevel);
    void draw(SDL_Renderer* renderer);

    // Afficher l'écran de transition
    void showWorldTransition(int worldNumber);
    bool isInTransition() const { return inTransition; }

    // Afficher l'écran de Game Over
    void showWorldGameOver(int worldNumber);
    bool isInGameOver() const { return inGameOver; }

private:
    TTF_Font* font;
    int margin;
    int windowHeight;
    int windowWidth;
    float button1Width;
    float button1Height;
    float button1X;
    float button1Y;
    float button2Width;
    float button2Height;
    float button2X;
    float button2Y;
    float button3Width;
    float button3Height;
    float button3X;
    float button3Y;
    float button4Width;
    float button4Height;
    float button4X;
    float button4Y;
    float transitionButtonWidth;
    float transitionButtonHeight;
    float spacingBetweenButtons;
    float centerX;
    float centerY;
    float menuButtonX;
    float menuButtonY;
    float nextButtonX;
    float nextButtonY;
    float GameOverButtonWidth;
    float GameOverButtonHeight;
    Button buttonLevel1;
    Button buttonLevel2;
    Button buttonLevel3;
    Button buttonLevel4;

    // Boutons pour l'écran de transition
    Button buttonBackToMenu;
    Button buttonNextWorld;

    // Etat de transition
    bool inTransition;
    bool inGameOver;
    int currentWorld;
    bool isFinalWorld;
};
