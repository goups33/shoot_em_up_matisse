#include "Menu.h"
#include <SDL3_image/SDL_image.h>

Menu::Menu(SDL_Window* window, TTF_Font* font, SDL_Renderer* renderer)
    : font(font), backgroundTexture(nullptr) {
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Charger l'image de fond
    loadBackground(renderer, "fond d'écran side début");

    // Dimensions des boutons
    float buttonWidth = 300;
    float buttonHeight = 80;
    float spacing = 30;

    // Centrer les boutons
    float centerX = windowWidth / 2.0f - buttonWidth / 2.0f;
    float centerY = windowHeight / 2.0f;

    // Bouton "Commencer"
    buttonPlay = createButton(
        centerX,
        centerY - buttonHeight - spacing / 2,
        buttonWidth,
        buttonHeight,
        "COMMENCER"
    );

    // Bouton "Quitter"
    buttonQuit = createButton(
        centerX,
        centerY + spacing / 2,
        buttonWidth,
        buttonHeight,
        "QUITTER"
    );

    // Couleurs personnalisées pour le bouton Quitter (rouge)
    buttonQuit.color.r = 180;
    buttonQuit.color.g = 70;
    buttonQuit.color.b = 70;

    buttonQuit.hoverColor.r = 220;
    buttonQuit.hoverColor.g = 100;
    buttonQuit.hoverColor.b = 100;
}

Menu::~Menu() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
}

bool Menu::loadBackground(SDL_Renderer* renderer, const char* imagePath) {
    SDL_Surface* surface = IMG_Load(imagePath);
    if (!surface) {
        SDL_Log("Erreur chargement image: %s", SDL_GetError());
        return false;
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (!backgroundTexture) {
        SDL_Log("Erreur création texture: %s", SDL_GetError());
        return false;
    }

    SDL_Log("Image de fond chargée avec succès!");
    return true;
}

void Menu::handleEvent(const SDL_Event& event, GameState& currentState) {
    handleButtonEvent(&buttonPlay, const_cast<SDL_Event*>(&event));
    handleButtonEvent(&buttonQuit, const_cast<SDL_Event*>(&event));

    if (isButtonClicked(&buttonPlay, const_cast<SDL_Event*>(&event))) {
        currentState = GameState::PLAYING;
    }
    else if (isButtonClicked(&buttonQuit, const_cast<SDL_Event*>(&event))) {
        currentState = GameState::QUIT;
    }
}

void Menu::draw(SDL_Renderer* renderer) {
    // Fond par défaut si l'image n'est pas chargée
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    SDL_RenderClear(renderer);

    // Dessiner l'image de fond si elle est chargée
    if (backgroundTexture) {
        SDL_FRect destRect = { 0, 0, (float)windowWidth, (float)windowHeight };
        SDL_RenderTexture(renderer, backgroundTexture, nullptr, &destRect);
    }

    // Dessiner les boutons
    renderButton(renderer, &buttonPlay, font);
    renderButton(renderer, &buttonQuit, font);

    // Titre du jeu (optionnel)
    if (font) {
        SDL_Color titleColor = { 255, 255, 255, 255 };
        SDL_Surface* titleSurface = TTF_RenderText_Blended(
            font,
            "MILES: INTERFECTOR ROBOTUM",
            0,
            titleColor
        );

        if (titleSurface) {
            SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            if (titleTexture) {
                float titleX = windowWidth / 2.0f - titleSurface->w / 2.0f;
                float titleY = 150;

                SDL_FRect titleRect = {
                    titleX,
                    titleY,
                    (float)titleSurface->w,
                    (float)titleSurface->h
                };

                SDL_RenderTexture(renderer, titleTexture, nullptr, &titleRect);
                SDL_DestroyTexture(titleTexture);
            }
            SDL_DestroySurface(titleSurface);
        }
    }
}