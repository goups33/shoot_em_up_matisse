#include "Button.h"

Button createButton(float x, float y, float w, float h, const char* text) {
    Button btn;

    btn.rect.x = x;
    btn.rect.y = y;
    btn.rect.w = w;
    btn.rect.h = h;

    btn.text = text;

    btn.color.r = 70;
    btn.color.g = 130;
    btn.color.b = 180;
    btn.color.a = 255;

    btn.hoverColor.r = 100;
    btn.hoverColor.g = 149;
    btn.hoverColor.b = 237;
    btn.hoverColor.a = 255;

    btn.textColor.r = 255;
    btn.textColor.g = 255;
    btn.textColor.b = 255;
    btn.textColor.a = 255;

    btn.isHovered = false;
    btn.isPressed = false;
    btn.wasPressedOn = false;

    return btn;
}

bool isPointInButton(Button* btn, float x, float y) {
    return x >= btn->rect.x && x <= btn->rect.x + btn->rect.w &&
        y >= btn->rect.y && y <= btn->rect.y + btn->rect.h;
}

void handleButtonEvent(Button* btn, SDL_Event* e) {
    if (e->type == SDL_EVENT_MOUSE_MOTION) {
        float x = e->motion.x;
        float y = e->motion.y;
        btn->isHovered = isPointInButton(btn, x, y);
    }
    else if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN && e->button.button == SDL_BUTTON_LEFT) {
        float x = e->button.x;
        float y = e->button.y;
        if (isPointInButton(btn, x, y)) {
            btn->isPressed = true;
            btn->wasPressedOn = true;
        }
    }
    else if (e->type == SDL_EVENT_MOUSE_BUTTON_UP && e->button.button == SDL_BUTTON_LEFT) {
        btn->isPressed = false;
    }
}

bool isButtonClicked(Button* btn, SDL_Event* e) {
    if (e->type == SDL_EVENT_MOUSE_BUTTON_UP && e->button.button == SDL_BUTTON_LEFT) {
        float x = e->button.x;
        float y = e->button.y;

        if (btn->wasPressedOn && isPointInButton(btn, x, y)) {
            btn->wasPressedOn = false;
            return true;
        }
        btn->wasPressedOn = false;
    }
    return false;
}

void renderButton(SDL_Renderer* renderer, Button* btn, TTF_Font* font) {
    SDL_Color currentColor = btn->isHovered ? btn->hoverColor : btn->color;

    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderFillRect(renderer, &btn->rect);

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderRect(renderer, &btn->rect);

    if (font && !btn->text.empty()) {
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, btn->text.c_str(), 0, btn->textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                float textX = btn->rect.x + (btn->rect.w - textSurface->w) / 2;
                float textY = btn->rect.y + (btn->rect.h - textSurface->h) / 2;

                SDL_FRect textRect = { textX,textY,(float)textSurface->w, (float)textSurface->h };
                SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_DestroySurface(textSurface);
        }
    }
}
