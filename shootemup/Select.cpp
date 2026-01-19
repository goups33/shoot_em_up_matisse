#include "Select.h"

Select::Select(SDL_Window* window, TTF_Font* font) : font(font), inTransition(false), inGameOver(false) {
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    margin = 20;

    // Bouton de transition
    transitionButtonWidth = windowHeight * 0.3;
    transitionButtonHeight = windowHeight * 0.1;
    spacingBetweenButtons = windowHeight * 0.5;

    centerX = windowWidth / 2;
    centerY = windowHeight / 2;

    menuButtonX = centerX - transitionButtonWidth / 2;
    menuButtonY = centerY - transitionButtonHeight - spacingBetweenButtons / 2;
    buttonBackToMenu = createButton(menuButtonX, menuButtonY, transitionButtonWidth, transitionButtonHeight, "Menu");

    nextButtonX = centerX - transitionButtonWidth / 2;
    nextButtonY = centerY + spacingBetweenButtons / 2;
    buttonNextWorld = createButton(nextButtonX, nextButtonY, transitionButtonWidth, transitionButtonHeight, "Next World");

    // Bouton de Game Over
    GameOverButtonWidth = windowHeight * 0.3;
    GameOverButtonHeight = windowHeight * 0.1;

    centerX = windowWidth / 2;
    centerY = windowHeight / 2;

    menuButtonX = centerX - GameOverButtonWidth / 2;
    menuButtonY = centerY - GameOverButtonHeight / 2;
    buttonBackToMenu = createButton(menuButtonX, menuButtonY, GameOverButtonWidth, GameOverButtonHeight, "Menu");
}

void Select::showWorldTransition(int worldNumber) {
    inTransition = true;
    currentWorld = worldNumber;
}

void Select::showWorldGameOver(int worldNumber)
{
    inGameOver = true;
    currentWorld = worldNumber;
}

void Select::handleEvent(const SDL_Event& event, int& selectedLevel) {
    if (inTransition) {
        // Gérer les evenements de l'écran de transition
        handleButtonEvent(&buttonBackToMenu, const_cast<SDL_Event*>(&event));
        handleButtonEvent(&buttonNextWorld, const_cast<SDL_Event*>(&event));

        if (isButtonClicked(&buttonBackToMenu, const_cast<SDL_Event*>(&event))) {
            inTransition = false;
            selectedLevel = -1; // Code spécial pour retour au menu
        }
        else if (isButtonClicked(&buttonNextWorld, const_cast<SDL_Event*>(&event))) {
            inTransition = false;
            selectedLevel = 1 + currentWorld;
        }
    }

    
}

void Select::draw(SDL_Renderer* renderer) {
    if (inTransition) {
        // Dessiner l'écran de transition
        SDL_SetRenderDrawColorFloat(renderer, 0.2f, 0.2f, 0.3f, 1.0f);
        SDL_RenderFillRect(renderer, nullptr);

        // Dessiner les boutons
        renderButton(renderer, &buttonBackToMenu, font);
        renderButton(renderer, &buttonNextWorld, font);
    }
    else if (inGameOver) {
        // Dessiner l'écran de Game over
        SDL_SetRenderDrawColorFloat(renderer, 0.2f, 0.2f, 0.3f, 1.0f);
        SDL_RenderFillRect(renderer, nullptr);

        // Dessiner le bouton
        renderButton(renderer, &buttonBackToMenu, font);
    }
    else {
        // Dessiner l'ecran de selection normal
        SDL_SetRenderDrawColorFloat(renderer, 1.0f, 1.0f, 0.0f, 1.0f);
        SDL_RenderFillRect(renderer, nullptr);

    }
}