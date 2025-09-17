#include "render/renderObstacle.h"

void RenderObstacle::draw(const Obstacle& obstacle, SDL_Renderer* renderer, const Camera& cam) {
    SDL_Rect rect {
        obstacle.x - cam.x,
        obstacle.y - cam.y,
        obstacle.width,
        obstacle.height
    };

    SDL_SetRenderDrawColor(renderer, 139, 139, 139, 255); 
    SDL_RenderFillRect(renderer, &rect);
}
