#ifndef RENDERENEMY_H
#define RENDERENEMY_H

#include <SDL2/SDL.h>
#include "../logic/enemy.h"
#include "../logic/camera.h"

#include <map>
#include <string>
#include <SDL2/SDL_image.h>

class RenderEnemy {
public:
    RenderEnemy() = default;
    ~RenderEnemy();

    void draw(const Enemy& enemy, SDL_Renderer* renderer, const Camera& cam, int worldWidth, int worldHeight);


    void loadTextures(SDL_Renderer* renderer) {
        textures["normal"] = loadTexture("assets/DO Monsters/Monsters/Zombie.PNG", renderer);
        textures["fast"]   = loadTexture("assets/DO Monsters/Monsters/Wolf.PNG", renderer);
        textures["tank"]   = loadTexture("assets/DO Monsters/Monsters/TrollFighter.PNG", renderer);
        textures["ranged"] = loadTexture("assets/DO Monsters/Monsters/SkeletonMage.PNG", renderer);
    }

private:
    std::map<std::string, SDL_Texture*> textures;
    SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);
};


#endif
