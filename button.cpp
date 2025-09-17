#include "render/button.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

void Button::draw(SDL_Renderer* renderer, int mouseX, int mouseY) {
    SDL_SetRenderDrawColor(renderer,
        isHovered(mouseX, mouseY) ? hoverColor.r : color.r,
        isHovered(mouseX, mouseY) ? hoverColor.g : color.g,
        isHovered(mouseX, mouseY) ? hoverColor.b : color.b,
        255
    );
    SDL_RenderFillRect(renderer, &rect);

    if (!font) return;

    SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), {255,255,255,255});
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

    int texW = 0, texH = 0;
    SDL_QueryTexture(tex, NULL, NULL, &texW, &texH);
    SDL_Rect dst = { rect.x + (rect.w - texW)/2, rect.y + (rect.h - texH)/2, texW, texH };

    SDL_RenderCopy(renderer, tex, NULL, &dst);

    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}
