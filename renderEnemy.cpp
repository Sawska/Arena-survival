#include "render/renderEnemy.h"
#include "logic/fast_enemy.h"
#include "logic/tank_enemy.h"
#include "logic/ranged_enemy.h"


RenderEnemy::~RenderEnemy() {
    for (auto& [key, tex] : textures) {
        SDL_DestroyTexture(tex);
    }
}

SDL_Texture* RenderEnemy::loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        SDL_Log("Failed to load image %s: %s", path.c_str(), IMG_GetError());
        return nullptr;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
    SDL_Log("Failed to create texture from %s: %s", path.c_str(), SDL_GetError());
}

    return texture;
}

void RenderEnemy::draw(const Enemy& enemy, SDL_Renderer* renderer, const Camera& cam, int screenWidth, int screenHeight) {
    if (!enemy.alive) return;

    SDL_Texture* tex = nullptr;

    if (dynamic_cast<const FastEnemy*>(&enemy)) {
        tex = textures["fast"];      
    } else if (dynamic_cast<const TankEnemy*>(&enemy)) {
        tex = textures["tank"];       
    } else if (dynamic_cast<const RangedEnemy*>(&enemy)) {
    tex = textures["ranged"];       
    } else {
        tex = textures["normal"];      
    }

    if (!tex) {
        SDL_Rect fallback{ enemy.x - cam.x, enemy.y - cam.y, enemy.width, enemy.height };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &fallback);
        return;
    }

    SDL_Rect destRect{
        enemy.x - cam.x,
        enemy.y - cam.y,
        static_cast<int>(enemy.width * enemy.scale),
        static_cast<int>(enemy.height * enemy.scale)
    };
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, tex, nullptr, &destRect);
}

