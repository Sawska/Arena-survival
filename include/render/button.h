#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Button {
public:
    SDL_Rect rect;
    std::string text;
    SDL_Color color;
    SDL_Color hoverColor;
    TTF_Font* font;

    SDL_Texture* normalTex = nullptr;
    SDL_Texture* hoverTex  = nullptr;

    Button(int x, int y, int w, int h, const std::string& t,
           SDL_Color c = {100,100,255,255}, SDL_Color hc = {150,150,255,255}, TTF_Font* f = nullptr)
        : rect{x, y, w, h}, text(t), color(c), hoverColor(hc), font(f) {}

    void draw(SDL_Renderer* renderer, int mouseX, int mouseY);
    bool isHovered(int mouseX, int mouseY) const {
        return mouseX >= rect.x && mouseX <= rect.x + rect.w &&
               mouseY >= rect.y && mouseY <= rect.y + rect.h;
    }
    const std::string& getText() const { return text; }
};

#endif
