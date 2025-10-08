#include "render/renderPlayer.h"
#include <iostream>

#include "render/renderPlayer.h"
#include <iostream>

RenderPlayer::RenderPlayer(Player* p) : player(p), texture(nullptr) {}

void RenderPlayer::loadTexture(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("assets/BarbarianPriest.PNG");
    if (!surface) {
        std::cerr << "Failed to load player surface: " << IMG_GetError() << std::endl;
        return;
    }
    Uint32 colorkey = SDL_MapRGB(surface->format, 255, 0, 255);
    SDL_SetColorKey(surface, SDL_TRUE, colorkey);

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Failed to create player texture: " << SDL_GetError() << std::endl;
    }
}


RenderPlayer::~RenderPlayer() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void RenderPlayer::draw(const Player& player, SDL_Renderer* renderer, const Camera& cam, int screenWidth, int screenHeight) {
    int px = player.x - cam.x;
    int py = player.y - cam.y;

    SDL_Rect rect{ px, py, player.width, player.height };

    if (px + player.width >= 0 && px < screenWidth &&
        py + player.height >= 0 && py < screenHeight) 
    {
        if (texture) {
            SDL_RenderCopy(renderer, texture, nullptr, &rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
