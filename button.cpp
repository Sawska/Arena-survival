#include "render/button.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void Button::draw(SDL_Renderer* renderer, int mouseX, int mouseY) {
    bool hovered = isHovered(mouseX, mouseY);

    if (hovered && hoverTex) {
        SDL_RenderCopy(renderer, hoverTex, nullptr, &rect);
    } else if (normalTex) {
        SDL_RenderCopy(renderer, normalTex, nullptr, &rect);
    } else {
        SDL_Color drawColor = hovered ? hoverColor : color;
        SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    // draw text
    if (font) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), {255,255,255,255});
        if (surf) {
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_Rect dst = { rect.x + (rect.w - surf->w) / 2,
                             rect.y + (rect.h - surf->h) / 2,
                             surf->w, surf->h };
            SDL_RenderCopy(renderer, tex, nullptr, &dst);
            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
        }
    }
}
