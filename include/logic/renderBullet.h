#ifndef RENDERBULLET_H
#define RENDERBULLET_H

#include <SDL2/SDL.h>
#include "../logic/bullet.h"
#include "../logic/camera.h"

class RenderBullet {
public:
    RenderBullet() = default;


  void draw(const Bullet& bullet, SDL_Renderer* renderer, const Camera& cam, int worldWidth, int worldHeight);
};

#endif // RENDERBULLET_H
