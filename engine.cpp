#include "engine.h"
#include <iostream>

Engine::Engine()
    : window(nullptr), renderer(nullptr),
      running(false),
      player(100, 100),
      renderPlayer(&player),
      renderEnemy(),      
      spawner(enemies),
      state(GameState::RUNNING)    
{}

Engine::~Engine() {
    clean();
}

bool Engine::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL Init Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Game Engine",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer Error: " << SDL_GetError() << std::endl;
        return false;
    }
    running = true;
    return true;
}

void Engine::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
    }

    
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W]) player.move(0, -5);
    if (state[SDL_SCANCODE_S]) player.move(0, 5);
    if (state[SDL_SCANCODE_A]) player.move(-5, 0);
    if (state[SDL_SCANCODE_D]) player.move(5, 0);
    if (state[SDL_SCANCODE_ESCAPE]) running = false;
    
}

void Engine::update() {
    spawner.update();

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        it->moveTowards(player.x, player.y);

        if (it->hitPlayer(player.x, player.y)) {
            player.takeDamage(it->damage);
            std::cout << "Player hit! HP: " << player.hp << std::endl;
        }

        if (!it->alive) {
            player.addScore(10);
            std::cout << "Enemy defeated! Score: " << player.score << std::endl;
            it = enemies.erase(it); 
        } else {
            ++it; 
        }
    }

    if(player.isDead()) {
        std::cout << "Game Over! Final Score: " << player.score << std::endl;
        state = GameState::GAME_OVER;
    }
}

void Engine::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

    renderPlayer.draw(renderer);

    for (auto& enemy : enemies) {
        renderEnemy.draw(enemy, renderer);
    }


    SDL_RenderPresent(renderer);
}

void Engine::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::run() {
    if (!init()) return;

    while (running) {
        handleEvents();

        switch (state) {
            case GameState::MENU:
                // renderMenu();  
                break;
            case GameState::RUNNING:
                update();
                render();
                break;
            case GameState::GAME_OVER:
                renderGameOver();
                break;
        }

        SDL_Delay(16);
    }
}



void Engine::renderGameOver() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect { 100, 100, 600, 400 };
    SDL_RenderFillRect(renderer, &rect);



    SDL_RenderPresent(renderer);

    running = false;
}
