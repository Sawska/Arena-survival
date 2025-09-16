#include "engine.h"
#include <iostream>
#include <algorithm>

Engine::Engine()
    : window(nullptr), renderer(nullptr),
      running(false),
      player(100, 100),
      renderPlayer(&player),
      renderEnemy(),      
      spawner(enemies, &enemyBullets),
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
    bullets.reserve(100);
    enemies.reserve(50);

    running = true;
    return true;
}
void Engine::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;

       
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
SDL_GetMouseState(&mouseX, &mouseY);


int worldMouseX = mouseX + camera.x;
int worldMouseY = mouseY + camera.y;

float dx = worldMouseX - player.x;
float dy = worldMouseY - player.y;

            float length = sqrt(dx*dx + dy*dy);
            float speed = 8.0f;

   if (length != 0) {
    float vx = dx / length * speed;
    float vy = dy / length * speed;
    bullets.push_back(Bullet(player.x, player.y, vx, vy, 10));
}


        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_W]) player.move(0, -5);
    if (state[SDL_SCANCODE_S]) player.move(0, 5);
    if (state[SDL_SCANCODE_A]) player.move(-5, 0);
    if (state[SDL_SCANCODE_D]) player.move(5, 0);
    if (state[SDL_SCANCODE_ESCAPE]) running = false;
}


void Engine::update() {
    spawner.update(elapsedTime);

    updateCamera();

    for (auto& enemy : enemies) {
        enemy->moveTowards(player.x, player.y);
        if (enemy->hitPlayer(player)) {
            player.takeDamage(enemy->damage);
            std::cout << "Player hit! HP: " << player.hp << std::endl;
        }
    }

    for (auto& bullet : bullets) {
        bullet.update();

        for (auto& enemy : enemies) {
            if (enemy->alive && enemy->hitByBullet(bullet)) {
                enemy->takeDamage(bullet.damage);
                bullet.kill();
                break;
            }
        }
    }


bullets.erase(
    std::remove_if(bullets.begin(), bullets.end(),
        [this](const Bullet& b) {
            return !b.alive 
                || b.x < 0 || b.x > worldWidth 
                || b.y < 0 || b.y > worldHeight;
        }),
    bullets.end()
);
    for (auto& b : enemyBullets) b.update();

enemyBullets.erase(
    std::remove_if(enemyBullets.begin(), enemyBullets.end(),
        [this](const Bullet& b) {
            return !b.alive 
                || b.x < 0 || b.x > worldWidth 
                || b.y < 0 || b.y > worldHeight;
        }),
    enemyBullets.end()
);


for (auto& b : enemyBullets) {
    if (player.isAlive && player.checkCollision(b)) {
        player.takeDamage(b.damage);
        b.kill();
    }
}


    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!(*it)->alive) {
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

    renderPlayer.draw(player, renderer, camera);

    for (auto& enemy : enemies) {
        renderEnemy.draw(*enemy, renderer, camera); 
    }

    for (auto& bullet : bullets) {
        renderBullet.draw(bullet, renderer, camera);
    }

    for (auto& b : enemyBullets) {
        b.isEnemy = true;
    renderBullet.draw(b, renderer,camera);
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

     Uint32 lastTime = SDL_GetTicks();

    while (running) {
        handleEvents();
        Uint32 currentTime = SDL_GetTicks();
        Uint32 delta = currentTime - lastTime;
        lastTime = currentTime;

        elapsedTime += delta; 
        

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


float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}





void Engine::updateCamera() {


camera.x = lerp(camera.x, player.x - 400, 0.1f);
camera.y = lerp(camera.y, player.y - 300, 0.1f);
 


    camera.x = std::max(0, std::min(camera.x, worldWidth - 800));
    camera.y = std::max(0, std::min(camera.y, worldHeight - 600));
}
