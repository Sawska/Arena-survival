#include "render/renderPlayer.h"

RenderPlayer::RenderPlayer(Player* p) : player(p) {}




void RenderPlayer::draw(const Player& player, SDL_Renderer* renderer, const Camera& cam) {
    SDL_Rect rect { player.x - cam.x, player.y - cam.y, player.width, player.height };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}