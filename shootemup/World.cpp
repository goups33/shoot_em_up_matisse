#include "World.h"
#include <SDL3_image/SDL_image.h>

World::World(SDL_Renderer* renderer)
    : background1(nullptr), background2(nullptr)
{
    loadBackgrounds(renderer);
}

World::~World() {
    if (background1) {
        SDL_DestroyTexture(background1);
    }
    if (background2) {
        SDL_DestroyTexture(background2);
    }
}

bool World::loadBackgrounds(SDL_Renderer* renderer) {
    // Charger le premier fond
    const char* bg1Paths[] = {
        "Background_slide.PNG",
        "./Background_slide.PNG",
        "../Background_slide.PNG",
        "../../Background_slide.PNG",
        "./assets/Background_slide.PNG",
        "../assets/Background_slide.PNG"
    };

    for (const char* path : bg1Paths) {
        SDL_Surface* surface = IMG_Load(path);
        if (surface) {
            background1 = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            SDL_Log("Premier fond chargé depuis: %s", path);
            break;
        }
    }

    // Charger le deuxième fond
    const char* bg2Paths[] = {
        "Background_slide_end.PNG",
        "./Background_slide_end.PNG",
        "../Background_slide_end.PNG",
        "../../Background_slide_end.PNG",
        "./assets/Background_slide_end.PNG",
        "../assets/Background_slide_end.PNG"
    };

    for (const char* path : bg2Paths) {
        SDL_Surface* surface = IMG_Load(path);
        if (surface) {
            background2 = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            SDL_Log("Deuxième fond chargé depuis: %s", path);
            break;
        }
    }

    return background1 != nullptr;
}

void World::render(SDL_Renderer* renderer, const Camera& camera) {
    // Utiliser le deuxième fond à partir de la 11ème tuile (10 * 1920 pixels)
    auto tileIndex = (int)(camera.x / 1920.0f);

    // Offset pour le défilement
    auto startTileX = (int)(camera.x / 1920.0f);
    auto startTileY = (int)(camera.y / 1080.0f);

    float offsetX = -(camera.x - startTileX * 1920.0f);
    float offsetY = -(camera.y - startTileY * 1080.0f);

    // Dessiner les tuiles visibles
    for (int ty = 0; ty < 2; ty++) {
        for (int tx = 0; tx < 2; tx++) {
            // Choisir le bon fond selon la tuile
            int currentTileX = startTileX + tx;
            SDL_Texture* tileBg = (currentTileX >= 10 && background2) ?
                background2 : background1;

            if (tileBg) {
                SDL_FRect bgRect;
                bgRect.x = offsetX + tx * 1920.0f;
                bgRect.y = offsetY + ty * 1080.0f;
                bgRect.w = 1920.0f;
                bgRect.h = 1080.0f;
                SDL_RenderTexture(renderer, tileBg, nullptr, &bgRect);
            }
        }
    }
}

bool World::shouldTransition(float playerWorldX) const {
    // Transition quand le joueur atteint la fin de la 11ème tuile
    return playerWorldX >= 20500.0f;
}