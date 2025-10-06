#include "logic/renderBullet.h"

void RenderBullet::draw(const Bullet& bullet, SDL_Renderer* renderer, 
                        const Camera& cam, int screenWidth, int screenHeight,
                        SDL_Texture* playerBulletTexture, SDL_Texture* enemyBulletTexture)
{
    int size = bullet.isEnemy ? 20 : 30;
    int bx = static_cast<int>(bullet.x - cam.x - size / 4);
    int by = static_cast<int>(bullet.y - cam.y - size / 4);

    SDL_Rect rect{ bx, by, size, size };

    if (bx + size >= 0 && bx < screenWidth &&
        by + size >= 0 && by < screenHeight)
    {
        SDL_Texture* tex = bullet.isEnemy ? enemyBulletTexture : playerBulletTexture;
        if (tex)
            SDL_RenderCopy(renderer, tex, nullptr, &rect);
        else {

            SDL_SetRenderDrawColor(renderer, bullet.isEnemy ? 255 : 255,
                                              bullet.isEnemy ? 0   : 255,
                                              bullet.isEnemy ? 0   : 0,
                                              255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}


