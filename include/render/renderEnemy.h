#ifndef RENDERENEMY_H
#define RENDERENEMY_H

#include <SDL2/SDL.h>
#include "../logic/enemy.h"

class RenderEnemy {
public:
    RenderEnemy() = default; 
    void draw(const Enemy& enemy, SDL_Renderer* renderer);
};

#endif
