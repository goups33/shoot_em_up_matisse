#include "Player.h"
#include <SDL3_image/SDL_image.h>

Player::Player(SDL_Renderer* renderer)
    : worldX(1920.0f / 2.0f), worldY(1080.0f / 2.0f),
    width(40), height(40),
    idleSprite(nullptr),
    currentFrame(0),
    animationTime(0),
    animationSpeed(100.0f),  // 100ms entre chaque frame
    isWalking(false)
{
    loadSprites(renderer);
}

Player::~Player() {
    if (idleSprite) {
        SDL_DestroyTexture(idleSprite);
    }
    for (auto sprite : walkSprites) {
        if (sprite) {
            SDL_DestroyTexture(sprite);
        }
    }
}

bool Player::loadSprites(SDL_Renderer* renderer) {
    // Charger le sprite Idle
    const char* idlePaths[] = {
        "Idle.PNG",
        "./Idle.PNG",
        "../Idle.PNG",
        "../../Idle.PNG",
        "./assets/Idle.PNG",
        "../assets/Idle.PNG"
    };

    for (const char* path : idlePaths) {
        SDL_Surface* surface = IMG_Load(path);
        if (surface) {
            idleSprite = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            SDL_Log("Sprite Idle chargé depuis: %s", path);
            break;
        }
    }


    // Charger les sprites de marche
    const char* walkPaths[][6] = {
        {"marche_1.PNG", "./marche_1.PNG", "../marche_1.PNG", "../../marche_1.PNG", "./assets/marche_1.PNG", "../assets/marche_1.PNG"},
        {"marche_2.PNG", "./marche_2.PNG", "../marche_2.PNG", "../../marche_2.PNG", "./assets/marche_2.PNG", "../assets/marche_2.PNG"},
        {"marche_3.PNG", "./marche_3.PNG", "../marche_3.PNG", "../../marche_3.PNG", "./assets/marche_3.PNG", "../assets/marche_3.PNG"},
        {"marche_4.PNG", "./marche_4.PNG", "../marche_4.PNG", "../../marche_4.PNG", "./assets/marche_4.PNG", "../assets/marche_4.PNG"},
        {"marche_5.PNG", "./marche_5.PNG", "../marche_5.PNG", "../../marche_5.PNG", "./assets/marche_5.PNG", "../assets/marche_5.PNG"},
        {"marche_6.PNG", "./marche_6.PNG", "../marche_6.PNG", "../../marche_6.PNG", "./assets/marche_6.PNG", "../assets/marche_6.PNG"}
    };

    for (int i = 0; i < 6; i++) {
        SDL_Texture* walkTexture = nullptr;
        for (const char* path : walkPaths[i]) {
            SDL_Surface* surface = IMG_Load(path);
            if (surface) {
                walkTexture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_DestroySurface(surface);
                SDL_Log("Sprite marche_%d chargé depuis: %s", i + 1, path);
                break;
            }
        }
        walkSprites.push_back(walkTexture);
    }

    return idleSprite != nullptr;
}

void Player::updateAnimation(float deltaTime, bool isMoving) {
    if (isMoving) {
        isWalking = true;
        animationTime += deltaTime;

        if (animationTime >= animationSpeed) {
            animationTime = 0;
            currentFrame = (currentFrame + 1) % walkSprites.size();
        }
    }
    else {
        isWalking = false;
        currentFrame = 0;
        animationTime = 0;
    }
}

void Player::update(float deltaTime, bool movingLeft, bool movingRight, bool movingUp, bool movingDown) {
    bool isMoving = movingLeft || movingRight || movingUp || movingDown;
    updateAnimation(deltaTime, isMoving);
}

void Player::render(SDL_Renderer* renderer, const Camera& camera) {
    // Position à l'écran (toujours au centre horizontalement, en bas verticalement)
    SDL_FRect rect;
    rect.x = 1920.0f / 2.0f - width / 2.0f;
    rect.y = 1080.0f / 2.0f - height / 2.0f;
    rect.w = width;
    rect.h = height;

    // Choisir le sprite à afficher
    SDL_Texture* currentSprite = nullptr;

    if (isWalking && currentFrame < walkSprites.size() && walkSprites[currentFrame]) {
        currentSprite = walkSprites[currentFrame];
    }
    else if (idleSprite) {
        currentSprite = idleSprite;
    }

    if (currentSprite) {
        SDL_RenderTexture(renderer, currentSprite, nullptr, &rect);
    }
    else {
        // Fallback: carré rouge si pas de sprite
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}