#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include <algorithm>
#include <fstream>
#include <iostream>

#include "gestionballe.h"
#include "gestionevent.h"
#include "enemi.h"
#include "colision.h"
#include "Button.h"
#include "Select.h"
#include "GameState.h"
#include "Menu.h"
#include "Camera.h"

using namespace std;

int main(int argc, char** argv)
{
    srand(time(0));
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

    // Pour la v-sync, pas touche sinon jump de frames
    SDL_SetRenderVSync(renderer, 1);

    // Charger une police (vous devrez avoir un fichier .ttf)
    // Essayer plusieurs chemins possibles pour la police
    TTF_Font* font = TTF_OpenFont("arial.ttf", 32);
    if (!font) {
        font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 32);
    }
    if (!font) {
        font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 32);
    }
    else {
        SDL_Log("Police chargée avec succès!");
    }

    // CHARGER L'IMAGE DE FOND POUR LE JEU
    SDL_Texture* gameBackgroundTexture = nullptr;
    const char* backgroundPaths[] = {
        "Background_slide.PNG",
        "./Background_slide.PNG",
        "../Background_slide.PNG",
        "../../Background_slide.PNG",
        "./assets/Background_slide.PNG",
        "../assets/Background_slide.PNG"
    };

    SDL_Surface* bgSurface = nullptr;
    for (const char* path : backgroundPaths) {
        bgSurface = IMG_Load(path);
        if (bgSurface) {
            SDL_Log("Image de fond du jeu chargée depuis: %s", path);
            gameBackgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
            SDL_DestroySurface(bgSurface);
            break;
        }
    }

    // Machine d'état
    GameState currentState = GameState::MENU;

    // Initialiser le menu (passer le renderer maintenant)
    Menu menu(window, font, renderer);

    // Objets du jeu
    gestionballe gestion_b;
    gestionevent gestion_e;
    enemi gestion_enemi;
    colision gest_colision;
    Camera camera(1920.0f, 1080.0f, 10);
    SDL_FRect rectangle;
    rectangle.w = 40;
    rectangle.h = 40;

    bool keepGoing = true;
    float timePrev = 0;

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
                    // Traiter les événements du jeu
                    if (!gestion_e.UpdateEvents(&event)) {
                        keepGoing = false;
                    }
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
            // Logique du jeu
            float now = SDL_GetTicks();
            gestion_enemi.time = gestion_enemi.time + now - timePrev;

            if (float dt = now - timePrev; dt > 0.6) {
                timePrev = now;

                if (gestion_e.go_left)
                    camera.moveLeft(dt);
                if (gestion_e.go_right)
                    camera.moveRight(dt);
                if (gestion_e.go_up)
                    camera.moveUp(dt);
                if (gestion_e.go_down)
                    camera.moveDown(dt);

                rectangle.x = 1920.0f / 2.0f - rectangle.w / 2.0f;  // Centre X
                rectangle.y = 1800.0f / 2.0f - rectangle.h / 2.0f;  // Centre Y

                float playerWorldX = camera.getPlayerWorldX();
                float playerWorldY = camera.getPlayerWorldY();

                // Gestion balle avec direction
                if (gestion_e.shoot && gestion_e.canShoot(now)) {
                    gestion_b.shoobullet(
                        playerWorldX,
                        playerWorldY,
                        gestion_e.shootDirection);
                }
                gestion_b.Update_bullet(renderer);

                // Gestion enemi
                if (gestion_enemi.time >= gestion_enemi.spawn_cooldown)
                    gestion_enemi.spawn_enemi();
                gestion_enemi.Update_enemi(renderer);

                // Gestion des colisions des balles
                gest_colision.gestion_colision_balle(&gestion_b, &gestion_enemi);

            }

            // Dessiner le jeu
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Dessiner le fond répété (effet tuile)
            if (gameBackgroundTexture) {
                // Calculer combien de tuiles sont nécessaires
                int tilesX = (int)(camera.worldWidth / 1920.0f) + 1;
                int tilesY = (int)(camera.worldHeight / 1080.0f) + 1;

                // Offset pour le défilement
                auto startTileX = (int)(camera.x / 1920.0f);
                auto startTileY = (int)(camera.y / 1080.0f);

                float offsetX = -(camera.x - startTileX * 1920.0f);
                float offsetY = -(camera.y - startTileY * 1080.0f);

                // Dessiner les tuiles visibles
                for (int ty = 0; ty < 2; ty++) {
                    for (int tx = 0; tx < 2; tx++) {
                        SDL_FRect bgRect;
                        bgRect.x = offsetX + tx * 1920.0f;
                        bgRect.y = offsetY + ty * 1080.0f;
                        bgRect.w = 1920.0f;
                        bgRect.h = 1080.0f;
                        SDL_RenderTexture(renderer, gameBackgroundTexture, nullptr, &bgRect);
                    }
                }
            }

            // Dessiner les balles (convertir coordonnées monde vers écran)
            for (const auto& bullet : gestion_b.gestionbullet) {
                SDL_FRect bulletRect;
                bulletRect.x = camera.worldToScreenX(bullet.x);
                bulletRect.y = camera.worldToScreenY(bullet.y);
                bulletRect.w = 10;
                bulletRect.h = 5;

                SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer, &bulletRect);
            }

            // Dessiner les ennemis (convertir coordonnées monde vers écran)
            for (const auto& enemy : gestion_enemi.gestion_enemi) {
                SDL_FRect enemyRect;
                enemyRect.x = camera.worldToScreenX(enemy.x);
                enemyRect.y = camera.worldToScreenY(enemy.y);
                enemyRect.w = 20;
                enemyRect.h = 20;

                SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer, &enemyRect);
            }

            // Dessiner le personnage (toujours au centre)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rectangle);

            break;
        }

        default:
            break;
        }

        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    if (gameBackgroundTexture) {
        SDL_DestroyTexture(gameBackgroundTexture);
    }
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}