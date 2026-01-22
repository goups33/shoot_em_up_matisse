#include "Player.h"
#include <SDL3_image/SDL_image.h>

Player::Player(SDL_Renderer* renderer)
    : worldX(1920.0f / 2.0f), worldY(1080.0f / 2.0f),
    width(40), height(40),
    idleSprite(nullptr),
    attackSprite(nullptr),
    currentFrame(0),
    animationTime(0),
    animationSpeed(100.0f),
    isWalking(false),
    isAttacking(false),
    attackAnimationTime(0),
    attackAnimationDuration(300.0f)
{
    loadSprites(renderer);
}

Player::~Player() {
    if (idleSprite) {
        SDL_DestroyTexture(idleSprite);
    }
    if (attackSprite) {
        SDL_DestroyTexture(attackSprite);
    }
    for (auto sprite : walkSprites) {
        if (sprite) {
            SDL_DestroyTexture(sprite);
        }
    }
}

bool Player::loadSprites(SDL_Renderer* renderer) {
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

    // Charger le sprite d'attaque
    const char* attackPaths[] = {
        "attack.PNG",
        "./attack.PNG",
        "../attack.PNG",
        "../../attack.PNG",
        "./assets/attack.PNG",
        "../assets/attack.PNG"
    };

    for (const char* path : attackPaths) {
        SDL_Surface* surface = IMG_Load(path);
        if (surface) {
            attackSprite = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            SDL_Log("Sprite Attack chargé depuis: %s", path);
            break;
        }
    }

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

void Player::startAttack() {
    if (!isAttacking) {
        isAttacking = true;
        attackAnimationTime = 0;
    }
}

void Player::update(float deltaTime, bool movingLeft, bool movingRight, bool movingUp, bool movingDown) {
    // Gérer l'animation d'attaque
    if (isAttacking) {
        attackAnimationTime += deltaTime;
        if (attackAnimationTime >= attackAnimationDuration) {
            isAttacking = false;
            attackAnimationTime = 0;
        }
        // Pendant l'attaque, on ne peut pas se déplacer
        return;
    }

    // Déplacement normal
    if (movingLeft) {
        direction = PlayerDirection::Left;
    }
    else if (movingRight) {
        direction = PlayerDirection::Right;
    }
    else if (movingUp) {
        direction = PlayerDirection::Up;
    }
    else if (movingDown) {
        direction = PlayerDirection::Down;
    }
    else {
        direction = PlayerDirection::None;
    }

    bool isMoving = (direction != PlayerDirection::None);
    updateAnimation(deltaTime, isMoving);
}

void Player::render(SDL_Renderer* renderer, const Camera& camera) {
    SDL_FRect rect;
    rect.x = 1920.0f / 2.0f - width / 2.0f;
    rect.y = 1080.0f * 0.8f - height / 2.0f;
    rect.w = width * 4;
    rect.h = height * 4;

    SDL_Texture* currentSprite = nullptr;

    // Priorité à l'animation d'attaque
    if (isAttacking && attackSprite) {
        currentSprite = attackSprite;
    }
    else if (isWalking && currentFrame < walkSprites.size() && walkSprites[currentFrame]) {
        currentSprite = walkSprites[currentFrame];
    }
    else if (idleSprite) {
        currentSprite = idleSprite;
    }

    if (currentSprite) {
        SDL_FlipMode flip = SDL_FLIP_NONE;

        if (direction == PlayerDirection::Left) {
            flip = SDL_FLIP_HORIZONTAL;
        }
        else {
            flip = SDL_FLIP_NONE;
        }
        SDL_RenderTextureRotated(renderer, currentSprite, nullptr, &rect, 0, nullptr, flip);
    }
}

PlayerDirection Player::getDirection() const {
    return direction;
}

bool Player::getIsAttacking() const {
    return isAttacking;
}