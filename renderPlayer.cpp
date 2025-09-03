#include "render/renderPlayer.h"

RenderPlayer::RenderPlayer(Player* p) : player(p) {}

void RenderPlayer::draw(SDL_Renderer* renderer) {
    SDL_Rect rect{ player->x, player->y, 50, 50 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
    SDL_RenderFillRect(renderer, &rect);
}
