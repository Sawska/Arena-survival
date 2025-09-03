#ifndef RENDERPLAYER_H
#define RENDERPLAYER_H

#include <SDL2/SDL.h>
#include "../logic/player.h"

class RenderPlayer {
    Player* player;
public:
    RenderPlayer(Player* p);
    void draw(SDL_Renderer* renderer);
};

#endif
