#ifndef RENDERPLAYER_H
#define RENDERPLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include "../logic/player.h"
#include "../logic/camera.h"

class RenderPlayer {
    Player* player;
    SDL_Texture* texture;
public:
    RenderPlayer(Player* p);
    ~RenderPlayer();

    void loadTexture(SDL_Renderer* renderer);  
    void draw(const Player& player, SDL_Renderer* renderer, const Camera& cam, int worldWidth, int worldHeight);
};


#endif
