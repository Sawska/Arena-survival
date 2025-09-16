#include "render/renderEnemy.h"
#include "logic/fast_enemy.h"
#include "logic/tank_enemy.h"
#include "logic/ranged_enemy.h"

void RenderEnemy::draw(const Enemy& enemy, SDL_Renderer* renderer, const Camera& cam) {
    if (!enemy.alive) return;

    SDL_Rect rect { enemy.x - cam.x, enemy.y - cam.y, enemy.width, enemy.height };


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
