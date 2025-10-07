#ifndef RENDEROBSTACLE_H
#define RENDEROBSTACLE_H

#include <SDL2/SDL.h>
#include "logic/Obstacle.h"
#include "../logic/camera.h"

class RenderObstacle {
public:
    static void draw(const Obstacle& obstacle, SDL_Renderer* renderer, const Camera& cam, SDL_Texture* obstacleTexture);
    
};

#endif
