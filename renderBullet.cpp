#include "logic/renderBullet.h"

void RenderBullet::draw(const Bullet& bullet, SDL_Renderer* renderer, const Camera& cam, int screenWidth, int screenHeight) {
    SDL_SetRenderDrawColor(renderer, bullet.isEnemy ? 255 : 255,
                                      bullet.isEnemy ? 0   : 255,
                                      bullet.isEnemy ? 0   : 0,
                                      255);

    int size = bullet.isEnemy ? 10 : 5;
    int bx = static_cast<int>(bullet.x - cam.x);
    int by = static_cast<int>(bullet.y - cam.y);

    SDL_Rect rect{ bx, by, size, size };

    if (bx + size >= 0 && bx < screenWidth &&
        by + size >= 0 && by < screenHeight) 
    {
        SDL_RenderFillRect(renderer, &rect);
    }
}


