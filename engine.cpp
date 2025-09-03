#include "engine.h"


Engine::Engine() {
    renderer = new Renderer();
    player = new Player();
    player->reset();
}

Engine::~Engine() {
    delete renderer;
}

