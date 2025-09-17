#include "logic/renderBullet.h"

void RenderBullet::draw(const Bullet& bullet, SDL_Renderer* renderer, const Camera& cam, int worldWidth, int worldHeight) {
    if (bullet.isEnemy)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    int size = bullet.isEnemy ? 10 : 5;

    int bx = (static_cast<int>(bullet.x - cam.x + worldWidth)) % worldWidth;
    int by = (static_cast<int>(bullet.y - cam.y + worldHeight)) % worldHeight;

    SDL_Rect rect{ bx, by, size, size };
    SDL_RenderFillRect(renderer, &rect);


    if (bx + size > worldWidth) {
        rect.x = bx - worldWidth;
        SDL_RenderFillRect(renderer, &rect);
    }
    if (by + size > worldHeight) {
        rect.y = by - worldHeight;
        SDL_RenderFillRect(renderer, &rect);
    }
}

