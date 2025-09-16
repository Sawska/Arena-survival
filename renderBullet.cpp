#include "logic/renderBullet.h"

void RenderBullet::draw(const Bullet& bullet, SDL_Renderer* renderer, const Camera& cam) {
    
    if (bullet.isEnemy) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    }

    
    int size = bullet.isEnemy ? 10 : 5;

    SDL_Rect rect;
    rect.x = static_cast<int>(bullet.x - cam.x); 
    rect.y = static_cast<int>(bullet.y - cam.y); 
    rect.w = size;
    rect.h = size;

    SDL_RenderFillRect(renderer, &rect);
}
