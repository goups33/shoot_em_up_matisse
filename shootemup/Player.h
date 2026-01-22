#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Camera.h"
#include "GameState.h"

class Player {
public:
    explicit Player(SDL_Renderer* renderer);
    ~Player();

    void update(float deltaTime, bool movingLeft, bool movingRight, bool movingUp, bool movingDown);
    void render(SDL_Renderer* renderer, const Camera& camera);

    float getWorldX() const { return worldX; }
    float getWorldY() const { return worldY; }
    PlayerDirection getDirection() const;
    bool getIsAttacking() const;
    void startAttack();

    bool loadSprites(SDL_Renderer* renderer);

private:
    float worldX;
    float worldY;
    float width;
    float height;

    SDL_Texture* idleSprite;
    SDL_Texture* attackSprite;
    std::vector<SDL_Texture*> walkSprites;

    int currentFrame;
    float animationTime;
    float animationSpeed;
    bool isWalking;

    bool isAttacking;
    float attackAnimationTime;
    float attackAnimationDuration;

    PlayerDirection direction = PlayerDirection::None;
    void updateAnimation(float deltaTime, bool isMoving);
};