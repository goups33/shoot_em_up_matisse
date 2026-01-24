#include "World.h"
#include <SDL3_image/SDL_image.h>
#include <random>
#include <algorithm>

World::World(SDL_Renderer* renderer)
    : background1(nullptr), background2(nullptr), lanternTexture(nullptr)
{
    loadBackgrounds(renderer);
    loadLanternSprite(renderer);
    initLanterns();
}

World::~World() {
    if (background1) SDL_DestroyTexture(background1);
    if (background2) SDL_DestroyTexture(background2);
    if (lanternTexture) SDL_DestroyTexture(lanternTexture);
}

void World::loadLanternSprite(SDL_Renderer* renderer) {
    const char* lanternPaths[] = {
        "lanterne.PNG",
        "./lanterne.PNG",
        "../lanterne.PNG",
        "../../lanterne.PNG",
        "./assets/lanterne.PNG",
        "../assets/lanterne.PNG"
    };

    for (const char* path : lanternPaths) {
        SDL_Surface* surface = IMG_Load(path);
        if (surface) {
            lanternTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            break;
        }
    }
}

void World::initLanterns() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(0, 1920.0f * 10);
    std::uniform_real_distribution<float> speedDist(1.5f, 2.0f);
    std::uniform_real_distribution<float> ampDist(5.0f, 15.0f);
    std::uniform_real_distribution<float> oscSpeedDist(1.0f, 3.0f);

    for (int i = 0; i < 20; i++) {
        Lantern lantern;
        lantern.x = xDist(gen);
        lantern.baseY = 50.0f + (i % 3) * 10.0f;
        lantern.y = lantern.baseY;
        lantern.speedFactor = speedDist(gen);
        lantern.amplitude = ampDist(gen);
        lantern.speed_lanterne = oscSpeedDist(gen);
        lantern.time = static_cast<float>(i) * 0.5f;
        lanterns.push_back(lantern);
    }

    std::sort(lanterns.begin(), lanterns.end(),
        [](const Lantern& a, const Lantern& b) { return a.x < b.x; });
}

bool World::loadBackgrounds(SDL_Renderer* renderer) {
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
            break;
        }
    }

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
            break;
        }
    }

    return background1 != nullptr;
}

void World::renderLanterns(SDL_Renderer* renderer, const Camera& camera) {
    float lanternWidth = 60.0f;
    float lanternHeight = 80.0f;
    float currentTime = SDL_GetTicks() / 1000.0f;

    for (auto& lantern : lanterns) {
        lantern.time += 0.016f;
        lantern.y = lantern.baseY + sin(lantern.time * lantern.speed_lanterne) * lantern.amplitude;

        float parallaxX = camera.x * lantern.speedFactor;
        float screenX = lantern.x - parallaxX;

        if (screenX + lanternWidth >= 0 && screenX <= camera.viewWidth) {
            SDL_FRect lanternRect;
            lanternRect.x = screenX;
            lanternRect.y = lantern.y;
            lanternRect.w = lanternWidth;
            lanternRect.h = lanternHeight;

            if (lanternTexture) {
                float rotation = sin(lantern.time * lantern.speed_lanterne * 2.0f) * 3.0f;
                SDL_RenderTextureRotated(renderer, lanternTexture, nullptr, &lanternRect,
                    rotation, nullptr, SDL_FLIP_NONE);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 255, 200, 50, 255);
                SDL_RenderFillRect(renderer, &lanternRect);
                SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
                SDL_RenderRect(renderer, &lanternRect);
            }
        }
    }
}

void World::render(SDL_Renderer* renderer, const Camera& camera) {
    auto startTileX = (int)(camera.x / 1920.0f);
    auto startTileY = (int)(camera.y / 1080.0f);

    float offsetX = -(camera.x - startTileX * 1920.0f);
    float offsetY = -(camera.y - startTileY * 1080.0f);

    for (int ty = 0; ty < 2; ty++) {
        for (int tx = 0; tx < 2; tx++) {
            int currentTileX = startTileX + tx;
            SDL_Texture* tileBg = (currentTileX >= 10 && background2) ? background2 : background1;

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

    renderLanterns(renderer, camera);
}

bool World::shouldTransition(float playerWorldX) const {
    return playerWorldX >= 20500.0f;
}