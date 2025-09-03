#ifndef ENGINE_H

#define ENGINE_H

#include "renderer.h"
#include "player.h"

class Engine {
    public: 
    Engine();
    ~Engine();
    private:
    Renderer* renderer;
    Player* player;


};

#endif // ENGINE_H