#ifndef RENDERPLAYER_H
#define RENDERPLAYER_H

#include <SDL2/SDL.h>
#include "../logic/player.h"
#include "../logic/camera.h"


class RenderPlayer {
    Player* player;
public:
    RenderPlayer(Player* p);
    void draw(const Player& player, SDL_Renderer* renderer, const Camera& cam, int worldWidth, int worldHeight);
};

#endif
