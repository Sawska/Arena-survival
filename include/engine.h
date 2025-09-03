#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include "logic/player.h"
#include "render/renderPlayer.h"
#include "logic/enemy.h"
#include "render/renderEnemy.h"
#include "logic/spawner.h"



#include <vector>

enum class GameState {
    MENU,
    RUNNING,
    GAME_OVER
};


class Engine {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Player player;
    RenderPlayer renderPlayer;

    std::vector<Enemy> enemies;   
     Spawner spawner;             
    RenderEnemy renderEnemy;      

public:
    Engine();
    ~Engine();

    bool init();
    void handleEvents();
    void update();
    void render();
    void clean();
    void run();
    void renderGameOver();
     GameState state; 
};




#endif
