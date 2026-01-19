#pragma once
#include <SDL3/SDL.h>

class Camera
{
public:
    float x;  // Position X de la caméra
    float y;  // Position Y de la caméra
    float speed;  // Vitesse de défilement

    // Limites du monde (10 fois l'écran)
    float worldWidth;
    float worldHeight;

    // Taille de la vue (écran)
    float viewWidth;
    float viewHeight;

    Camera(float screenWidth, float screenHeight, int worldMultiplier = 10);

    // Déplacer la caméra
    void moveLeft(float dt);
    void moveRight(float dt);
    void moveUp(float dt);
    void moveDown(float dt);

    // Convertir position monde vers position écran
    float worldToScreenX(float worldX) const;
    float worldToScreenY(float worldY) const;

    // Convertir position écran vers position monde
    float screenToWorldX(float screenX) const;
    float screenToWorldY(float screenY) const;

    // Obtenir la position du personnage dans le monde
    float getPlayerWorldX() const;
    float getPlayerWorldY() const;
};