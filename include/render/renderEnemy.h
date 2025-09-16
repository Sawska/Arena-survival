#ifndef RENDERENEMY_H
#define RENDERENEMY_H

#include <SDL2/SDL.h>
#include "../logic/enemy.h"
#include "../logic/camera.h"

class RenderEnemy {
public:
    RenderEnemy() = default; 
    

void draw(const Enemy& enemy, SDL_Renderer* renderer, const Camera& cam);
};

#endif
