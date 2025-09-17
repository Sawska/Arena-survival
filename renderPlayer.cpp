#include "render/renderPlayer.h"

RenderPlayer::RenderPlayer(Player* p) : player(p) {}



void RenderPlayer::draw(const Player& player, SDL_Renderer* renderer, const Camera& cam, int worldWidth, int worldHeight) {
    int px = (player.x - cam.x + worldWidth) % worldWidth;
    int py = (player.y - cam.y + worldHeight) % worldHeight;

    SDL_Rect rect{ px, py, player.width, player.height };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    if (px + player.width > worldWidth) {
        rect.x = px - worldWidth;
        SDL_RenderFillRect(renderer, &rect);
    }
    if (py + player.height > worldHeight) {
        rect.y = py - worldHeight;
        SDL_RenderFillRect(renderer, &rect);
    }
}
