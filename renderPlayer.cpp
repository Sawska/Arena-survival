#include "render/renderPlayer.h"

RenderPlayer::RenderPlayer(Player* p) : player(p) {}



void RenderPlayer::draw(const Player& player, SDL_Renderer* renderer, const Camera& cam, int screenWidth, int screenHeight) {
    int px = player.x - cam.x;
    int py = player.y - cam.y;

    SDL_Rect rect{ px, py, player.width, player.height };

    if (px + player.width >= 0 && px < screenWidth &&
        py + player.height >= 0 && py < screenHeight) 
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
