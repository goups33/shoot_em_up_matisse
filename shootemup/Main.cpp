#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "GameState.h"
#include "Menu.h"
#include "Game.h"
#include <xiosbase>

int main(int argc, char** argv)
{
    srand(time(nullptr));
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_SetAppMetadata("SDL Test", "1.0", "games.anakata.test-sdl");
    if (!SDL_Init(SDL_INIT_VIDEO))
        return 1;

    if (!TTF_Init()) {
        SDL_Quit();
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("MILES: INTERFECTOR ROBOTUM ", 1920, 1080, SDL_WINDOW_FULLSCREEN, &window, &renderer))
        return 1;

    SDL_SetRenderLogicalPresentation(renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // Pour la v-sync
    SDL_SetRenderVSync(renderer, 1);

    // Charger une police
    TTF_Font* font = TTF_OpenFont("arial.ttf", 32);
    if (!font) {
        font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 32);
    }
    if (!font) {
        font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 32);
    }
    if (font) {
        SDL_Log("Police chargée avec succès!");
    }

    // Machine d'état
    GameState currentState = GameState::MENU;

    // Initialiser le menu
    Menu menu(window, font, renderer);

    // Initialiser le jeu
    Game* game = new Game(renderer, font);

    bool keepGoing = true;

    while (keepGoing && currentState != GameState::QUIT)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                keepGoing = false;
            }

            // Gérer les événements selon l'état
            switch (currentState) {
            case GameState::MENU:
                menu.handleEvent(event, currentState);

                // ESC quitte depuis le menu
                if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                    keepGoing = false;
                }
                break;

            case GameState::PLAYING:
                // ESC retourne au menu depuis le jeu
                if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                    currentState = GameState::MENU;
                }
                else {
                    game->handleEvent(event);
                }
                break;

            default:
                break;
            }
        }

        // Mettre à jour et dessiner selon l'état
        switch (currentState) {
        case GameState::MENU:
            // Dessiner le menu
            menu.draw(renderer);
            break;

        case GameState::PLAYING:
        {
            // Vérifier si on doit retourner au menu
            if (game->shouldReturnToMenu()) {
                currentState = GameState::MENU;
                game->resetReturnToMenu();
                // Recréer le jeu pour reset l'état
                delete game;
                game = new Game(renderer, font);
            }
            else {
                // Mettre à jour le jeu
                float deltaTime = SDL_GetTicks();
                game->update(deltaTime);

                // Dessiner le jeu
                game->render(renderer);
            }
            break;
        }

        default:
            break;
        }

        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    delete game;
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}