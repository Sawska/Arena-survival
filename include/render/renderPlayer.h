#ifndef RENDERPLAYER_H
#define RENDERPLAYER_H

#include "SDL2/SDL.h"

class RenderPlayer {
public:
    RenderPlayer();
    void draw(int x, int y, int playerWidth = 50, int playerHeight = 50, SDL_Renderer* renderer = nullptr);
    ~RenderPlayer();
};

#endif // RENDERPLAYER_H
