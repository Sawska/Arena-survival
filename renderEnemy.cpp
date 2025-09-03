#include "render/renderEnemy.h"


void RenderEnemy::draw(const Enemy& enemy, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); 
    SDL_Rect rect { enemy.x, enemy.y, 50, 50 };
    SDL_RenderFillRect(renderer, &rect);
}

