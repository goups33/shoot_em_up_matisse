#include "Game.h"
#include <SDL3_image/SDL_image.h>

Game::Game(SDL_Renderer* renderer, TTF_Font* font)
    : camera(1920.0f, 1080.0f, 10), bulletTexture(nullptr),
    player(nullptr),
    world(nullptr),
    font(font),
    showWorldTransition(false),
    returnToMenu(false),
    timePrev(0),
    renderer(renderer)
{
    player = new Player(renderer);
    world = new World(renderer);

    // Charger le sprite de balle
    loadBulletSprite();

    buttonContinue = createButton(1920.0f / 2.0f - 150, 1080.0f / 2.0f - 100, 300, 80, "CONTINUER");
    buttonQuitGame = createButton(1920.0f / 2.0f - 150, 1080.0f / 2.0f + 20, 300, 80, "QUITTER");
    buttonQuitGame.color.r = 180;
    buttonQuitGame.color.g = 70;
    buttonQuitGame.color.b = 70;
    buttonQuitGame.hoverColor.r = 220;
    buttonQuitGame.hoverColor.g = 100;
    buttonQuitGame.hoverColor.b = 100;
}

Game::~Game() {
    delete player;
    delete world;
    if (bulletTexture) {
        SDL_DestroyTexture(bulletTexture);
    }
}

void Game::loadBulletSprite() {
    const char* bulletPaths[] = {
        "balle.PNG",
        "./balle.PNG",
        "../balle.PNG",
        "../../balle.PNG",
        "./assets/balle.PNG",
        "../assets/balle.PNG"
    };

    for (const char* path : bulletPaths) {
        SDL_Surface* surface = IMG_Load(path);
        if (surface) {
            bulletTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            SDL_Log("Sprite balle chargé depuis: %s", path);
            break;
        }
    }
}

void Game::handleEvent(SDL_Event& event) {
    if (showWorldTransition) {
        handleButtonEvent(&buttonContinue, &event);
        handleButtonEvent(&buttonQuitGame, &event);

        if (isButtonClicked(&buttonContinue, &event)) {
            showWorldTransition = false;
            camera.x = 0;
            camera.y = 0;
        }
        else if (isButtonClicked(&buttonQuitGame, &event)) {
            returnToMenu = true;
            showWorldTransition = false;
        }
    }
    else {
        gestion_e.UpdateEvents(&event);
    }
}

void Game::update(float deltaTime) {
    if (showWorldTransition) {
        return;
    }

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

        player->update(dt, gestion_e.go_left, gestion_e.go_right, gestion_e.go_up, gestion_e.go_down);

        float playerWorldX = camera.getPlayerWorldX();
        float playerWorldY = camera.getPlayerWorldY();

        // Déterminer la direction de tir automatiquement
        int shootDir = gestion_e.shootDirection;
        PlayerDirection playerDir = player->getDirection();

        // Si le joueur se déplace, tirer dans cette direction
        if (playerDir == PlayerDirection::Left) {
            shootDir = 2; // Gauche
        }
        else if (playerDir == PlayerDirection::Right) {
            shootDir = 0; // Droite
        }
        else if (playerDir == PlayerDirection::Up) {
            shootDir = 1; // Haut
        }
        else if (playerDir == PlayerDirection::Down) {
            shootDir = 3; // Bas
        }

        // Gestion du tir
        if (gestion_e.shoot && gestion_e.canShoot(now)) {
            gestion_b.shoobullet(playerWorldX, playerWorldY, shootDir);
        }

        gestion_b.Update_bullet(nullptr, camera.x, camera.y, camera.viewWidth, camera.viewHeight);

        // Gestion ennemis
        if (gestion_enemi.time >= gestion_enemi.spawn_cooldown)
            gestion_enemi.spawn_enemi();
        gestion_enemi.Update_enemi(nullptr);

        // Gestion des collisions
        gest_colision.gestion_colision_balle(&gestion_b, &gestion_enemi);

        if (world->shouldTransition(playerWorldX)) {
            showWorldTransition = true;
        }
    }
}

void Game::renderTransition(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    SDL_RenderClear(renderer);

    if (font) {
        SDL_Color textColor = { 255, 255, 255, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, "DEUXIEME MONDE", 0, textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                float textX = 1920.0f / 2.0f - textSurface->w / 2.0f;
                float textY = 200;
                SDL_FRect textRect = { textX, textY, (float)textSurface->w, (float)textSurface->h };
                SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_DestroySurface(textSurface);
        }
    }

    renderButton(renderer, &buttonContinue, font);
    renderButton(renderer, &buttonQuitGame, font);
}

void Game::renderGame(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    world->render(renderer, camera);

    // Dessiner les balles avec le sprite
    for (const auto& bullet : gestion_b.gestionbullet) {
        SDL_FRect bulletRect;
        bulletRect.x = camera.worldToScreenX(bullet.x);
        bulletRect.y = camera.worldToScreenY(bullet.y);
        bulletRect.w = 20;  // Taille du sprite de balle
        bulletRect.h = 20;

        if (bulletTexture) {
            // Calculer l'angle de rotation basé sur la vélocité
            float angle = atan2(bullet.velocityY, bullet.velocityX) * (180.0f / M_PI);
            SDL_RenderTextureRotated(renderer, bulletTexture, nullptr, &bulletRect, angle, nullptr, SDL_FLIP_NONE);
        }
        else {
            // Fallback si le sprite n'est pas chargé
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &bulletRect);
        }
    }

    // Dessiner les ennemis
    for (const auto& enemy : gestion_enemi.gestion_enemi) {
        SDL_FRect enemyRect;
        enemyRect.x = camera.worldToScreenX(enemy.x);
        enemyRect.y = camera.worldToScreenY(enemy.y);
        enemyRect.w = 20;
        enemyRect.h = 20;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &enemyRect);
    }

    player->render(renderer, camera);
}

void Game::render(SDL_Renderer* renderer) {
    if (showWorldTransition) {
        renderTransition(renderer);
    }
    else {
        renderGame(renderer);
    }
}