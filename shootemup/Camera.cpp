#include "Camera.h"
#include <algorithm>

Camera::Camera(float screenWidth, float screenHeight, int worldMultiplier)
    : x(0), y(0), speed(5.0f)
{
    viewWidth = screenWidth;
    viewHeight = screenHeight;

    // Le monde fait 10 fois la taille de l'écran
    worldWidth = screenWidth * worldMultiplier;
    worldHeight = screenHeight * worldMultiplier;


    x = (0);
    y = (0);
}

void Camera::moveLeft(float [[maybe_unused]] ) {
    x -= speed;
    // Limiter aux bords du monde
    x = std::max(0.0f, x);
}

void Camera::moveRight(float [[maybe_unused]] ) {
    x += speed;
    // Limiter aux bords du monde
    x = std::min(worldWidth - viewWidth, x);
}

void Camera::moveUp(float [[maybe_unused]] ) {
    y -= speed;
    // Limiter aux bords du monde
    y = std::max(0.0f, y);
}

void Camera::moveDown(float [[maybe_unused]] ) {
    y += speed;
    // Limiter aux bords du monde
    y = std::min(worldHeight - viewHeight, y);
}

float Camera::worldToScreenX(float worldX) const {
    return worldX - x;
}

float Camera::worldToScreenY(float worldY) const {
    return worldY - y;
}

float Camera::screenToWorldX(float screenX) const {
    return screenX + x;
}

float Camera::screenToWorldY(float screenY) const {
    return screenY + y;
}

float Camera::getPlayerWorldX() const {
    // Le joueur est toujours au centre de l'écran
    return x + viewWidth / 2.0f;
}

float Camera::getPlayerWorldY() const {
    // Le joueur est toujours au centre de l'écran
    return y + viewHeight / 2.0f;
}