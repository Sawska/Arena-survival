#include "render/renderEnemy.h"
#include "logic/fast_enemy.h"
#include "logic/tank_enemy.h"
#include "logic/ranged_enemy.h"

void RenderEnemy::draw(const Enemy& enemy, SDL_Renderer* renderer, const Camera& cam, int screenWidth, int screenHeight) {
    if (!enemy.alive) return;

    int ex = enemy.x - cam.x;
    int ey = enemy.y - cam.y;

    SDL_Rect rect{ ex, ey, enemy.width, enemy.height };

    if (ex + enemy.width >= 0 && ex < screenWidth &&
        ey + enemy.height >= 0 && ey < screenHeight) 
    {
        if (dynamic_cast<const FastEnemy*>(&enemy))
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else if (dynamic_cast<const TankEnemy*>(&enemy))
            SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        else if (dynamic_cast<const RangedEnemy*>(&enemy))
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        SDL_RenderFillRect(renderer, &rect);
    }
}
