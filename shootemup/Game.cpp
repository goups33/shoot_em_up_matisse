#include "Game.h"

Game::Game(SDL_Renderer* renderer, TTF_Font* font)
    : camera(1920.0f, 1080.0f, 10),
    player(nullptr),
    world(nullptr),
    font(font),
    showWorldTransition(false),
    returnToMenu(false),
    timePrev(0)
{
    // Créer le joueur et le monde
    player = new Player(renderer);
    world = new World(renderer);

    // Créer les boutons de transition
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
}

void Game::handleEvent(SDL_Event& event) {
    if (showWorldTransition) {
        // Gérer les événements de l'écran de transition
        handleButtonEvent(&buttonContinue, &event);
        handleButtonEvent(&buttonQuitGame, &event);

        if (isButtonClicked(&buttonContinue, &event)) {
            showWorldTransition = false;
            // Réinitialiser la caméra pour le nouveau monde
            camera.x = 0;
            camera.y = 0;
        }
        else if (isButtonClicked(&buttonQuitGame, &event)) {
            returnToMenu = true;
            showWorldTransition = false;
        }
    }
    else {
        // Traiter les événements du jeu
        gestion_e.UpdateEvents(&event);
    }
}

void Game::update(float deltaTime) {
    if (showWorldTransition) {
        return; // Pas de mise à jour pendant la transition
    }

    float now = SDL_GetTicks();
    gestion_enemi.time = gestion_enemi.time + now - timePrev;

    if (float dt = now - timePrev; dt > 0.6) {
        timePrev = now;

        // Déplacer la caméra
        if (gestion_e.go_left)
            camera.moveLeft(dt);
        if (gestion_e.go_right)
            camera.moveRight(dt);
        if (gestion_e.go_up)
            camera.moveUp(dt);
        if (gestion_e.go_down)
            camera.moveDown(dt);

        // Mettre à jour le joueur
        player->update(dt, gestion_e.go_left, gestion_e.go_right, gestion_e.go_up, gestion_e.go_down);

        float playerWorldX = camera.getPlayerWorldX();
        float playerWorldY = camera.getPlayerWorldY();

        // Gestion balle avec direction
        if (gestion_e.shoot && gestion_e.canShoot(now)) {
            gestion_b.shoobullet(
                playerWorldX,
                playerWorldY,
                gestion_e.shootDirection);
        }
        gestion_b.Update_bullet(nullptr, camera.x, camera.y, camera.viewWidth, camera.viewHeight);

        // Gestion ennemis
        if (gestion_enemi.time >= gestion_enemi.spawn_cooldown)
            gestion_enemi.spawn_enemi();
        gestion_enemi.Update_enemi(nullptr);

        // Gestion des collisions
        gest_colision.gestion_colision_balle(&gestion_b, &gestion_enemi);

        // Vérifier si on doit passer à l'écran de transition
        if (world->shouldTransition(playerWorldX)) {
            showWorldTransition = true;
        }
    }
}

void Game::renderTransition(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    SDL_RenderClear(renderer);

    // Afficher le texte "DEUXIEME MONDE"
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

    // Afficher les boutons
    renderButton(renderer, &buttonContinue, font);
    renderButton(renderer, &buttonQuitGame, font);
}

void Game::renderGame(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Dessiner le monde (fonds d'écran)
    world->render(renderer, camera);

    // Dessiner les balles
    for (const auto& bullet : gestion_b.gestionbullet) {
        SDL_FRect bulletRect;
        bulletRect.x = camera.worldToScreenX(bullet.x);
        bulletRect.y = camera.worldToScreenY(bullet.y);
        bulletRect.w = 10;
        bulletRect.h = 5;

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &bulletRect);
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

    // Dessiner le joueur
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