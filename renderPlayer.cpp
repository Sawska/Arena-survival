#include "renderPlayer.h"

RenderPlayer::RenderPlayer()
{

}

void RenderPlayer::draw(int x, int y, int playerWidth, int playerHeight, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer,255,0,0,255);

    SDL_Rect playerRect;

    playerRect.x = x;
    playerRect.y = y;
    playerRect.w = playerWidth;
    playerRect.h = playerHeight;

    SDL_RenderFillRect(renderer,&playerRect);

    SDL_RenderPresent(renderer);
}

RenderPlayer::~RenderPlayer()
{
}
