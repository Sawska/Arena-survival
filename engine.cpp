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
      state(GameState::MENU)
{}

Engine::~Engine() {
    clean();
}

bool Engine::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0) {
        std::cout << "TTF Init Error: " << TTF_GetError() << std::endl;
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

    font = TTF_OpenFont("/home/alex/Documents/Projects/game-engine/assets/font/OpenSans-Italic-VariableFont_wdth,wght.ttf", 24);
    if (!font) {
        std::cout << "Font load error: " << TTF_GetError() << std::endl;
        return false;
    }

    menuButtons.push_back(Button(300, 200, 200, 50, "Start", {0,0,255}, {50,50,255}, font));
    menuButtons.push_back(Button(300, 300, 200, 50, "Exit", {255,0,0}, {255,50,50}, font));


    gameOverButtons.push_back(Button(300, 200, 200, 50, "Retry", {0,255,0}, {50,255,50}, font));
    gameOverButtons.push_back(Button(300, 300, 200, 50, "Exit", {255,0,0}, {255,50,50}, font));


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

            if (state == GameState::MENU) {
                for (auto& btn : menuButtons) {
                    if (btn.isHovered(mouseX, mouseY)) {
                        if (btn.getText() == "Start") {
                            state = GameState::RUNNING;
                        } else if (btn.getText() == "Exit") {
                            running = false;
                        }
                    }
                }
            } 
            else if (state == GameState::GAME_OVER) {
                for (auto& btn : gameOverButtons) {
                    if (btn.isHovered(mouseX, mouseY)) {
                        if (btn.getText() == "Retry") {
                            player = Player(100, 100);
                            enemies.clear();
                            bullets.clear();
                            enemyBullets.clear();
                            obstacles = obstacleSpawner.spawnRandomObstacles(10, worldWidth, worldHeight, 20, 60);
                            elapsedTime = 0;
                            state = GameState::RUNNING;
                        } else if (btn.getText() == "Exit") {
                            running = false;
                        }
                    }
                }
            } 
            else if (state == GameState::RUNNING) {
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
            } else if(state == GameState::LEVEL_UP) {
                handleLevelUpEvent(event);
            }
        }
    }

    if (state == GameState::RUNNING) {
        int oldX = player.x;
        int oldY = player.y;

        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        if (keystate[SDL_SCANCODE_A]) player.move(-5, 0, worldWidth, worldHeight);
        if (keystate[SDL_SCANCODE_D]) player.move(5, 0, worldWidth, worldHeight);

        for (const auto& obstacle : obstacles) {
            if (obstacle.collidesWith(player.x, player.y, player.width, player.height)) {
                player.x = oldX;
                break;
            }
        }

        if (keystate[SDL_SCANCODE_W]) player.move(0, -5, worldWidth, worldHeight);
        if (keystate[SDL_SCANCODE_S]) player.move(0, 5, worldWidth, worldHeight);

        for (const auto& obstacle : obstacles) {
            if (obstacle.collidesWith(player.x, player.y, player.width, player.height)) {
                player.y = oldY;
                break;
            }
        }

        if (keystate[SDL_SCANCODE_ESCAPE]) running = false;
    }
}



void Engine::update() {
    if(state != GameState::RUNNING) return;

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

    if (player.level > lastPlayerLevel) {
        lastPlayerLevel = player.level;
        triggerLevelUp();
    }

    if(player.isDead()) {
        finalElapsedTime = elapsedTime;
        std::cout << "Game Over! Final Score: " << player.score << std::endl;
        state = GameState::GAME_OVER;
    }
}



void Engine::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

    renderPlayer.draw(player, renderer, camera, worldWidth, worldHeight);

    for (auto& enemy : enemies) {
        renderEnemy.draw(*enemy, renderer, camera, worldWidth, worldHeight); 
    }

    for (auto& bullet : bullets) {
        renderBullet.draw(bullet, renderer, camera, worldWidth, worldHeight);
    }

    for (auto& b : enemyBullets) {
        b.isEnemy = true;
    renderBullet.draw(b, renderer,camera, worldWidth, worldHeight);
}

    for(const auto& obstacle : obstacles) {
        RenderObstacle::draw(obstacle, renderer, camera);
    }

    if(state == GameState::LEVEL_UP) {
        renderLevelUpScreen();
        return; 
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

     obstacles = obstacleSpawner.spawnRandomObstacles(20, worldWidth, worldHeight, 50, 150);

    while (running) {
        handleEvents();
        Uint32 currentTime = SDL_GetTicks();
        Uint32 delta = currentTime - lastTime;
        lastTime = currentTime;

        elapsedTime += delta; 
        

        switch (state) {
            case GameState::MENU:
                renderMenu();  
                break;
            case GameState::RUNNING:
                update();
                render();
                break;
            case GameState::GAME_OVER:
                renderGameOver();
                break;
            case GameState::LEVEL_UP:
            render();
                break;
        }

        SDL_Delay(16);
    }
}



void Engine::renderGameOver() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect panel { 100, 100, 600, 400 };
    SDL_RenderFillRect(renderer, &panel);

    SDL_Color white = {255, 255, 255, 255};
    if (font) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, "Game Over", white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst { 300, 120, surf->w, surf->h };
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);

        int seconds = finalElapsedTime / 1000;
        std::string infoText = "You survived: " + std::to_string(seconds) + "s | "
                               "Level: " + std::to_string(player.level) + " | "
                               "Score: " + std::to_string(player.score);

        SDL_Surface* surf2 = TTF_RenderText_Solid(font, infoText.c_str(), white);
        SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);
        SDL_Rect dst2 { 150, 180, surf2->w, surf2->h }; 
        SDL_RenderCopy(renderer, tex2, nullptr, &dst2);
        SDL_FreeSurface(surf2);
        SDL_DestroyTexture(tex2);
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (auto& btn : gameOverButtons) {
        btn.draw(renderer, mouseX, mouseY);
    }

    SDL_RenderPresent(renderer);
}


void Engine::renderMenu() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect panel { 100, 100, 600, 400 };
    SDL_RenderFillRect(renderer, &panel);

    SDL_Color white = {255, 255, 255, 255};
    if (font) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, "My Game", white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst { 300, 120, surf->w, surf->h };
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }


    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (auto& btn : menuButtons) {
        btn.draw(renderer, mouseX, mouseY);
    }

    SDL_RenderPresent(renderer);
}


float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}





void Engine::updateCamera() {


camera.x = lerp(camera.x, player.x - 400, 0.1f);
camera.y = lerp(camera.y, player.y - 300, 0.1f);
 


    camera.x = std::max(0, std::min(camera.x, worldWidth - 800));
    camera.y = std::max(0, std::min(camera.y, worldHeight - 600));

       if (camera.x < 0) camera.x += worldWidth;
    if (camera.x >= worldWidth) camera.x -= worldWidth;
    if (camera.y < 0) camera.y += worldHeight;
    if (camera.y >= worldHeight) camera.y -= worldHeight;
}


void Engine::triggerLevelUp() {
    state = GameState::LEVEL_UP;
    currentChoices = player.getRandomSkillChoices();
}


void Engine::renderLevelUpScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);  
    SDL_Rect overlay = {0, 0, 800, 600};

    SDL_RenderFillRect(renderer, &overlay);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surf = TTF_RenderText_Solid(font, "Choose a Skill", white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect dst = {250, 50, surf->w, surf->h};
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    int y = 150;
    for (int i = 0; i < currentChoices.size(); i++) {
        SDL_Rect box = {150, y, 500, 80};
        SDL_SetRenderDrawColor(renderer, 50, 50, 100, 255);
        SDL_RenderFillRect(renderer, &box);

        auto& skill = currentChoices[i];
        SDL_Surface* surf2 = TTF_RenderText_Solid(font, skill.name.c_str(), white);
        SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);
        SDL_Rect dst2 = {box.x + 20, box.y + 20, surf2->w, surf2->h};
        SDL_RenderCopy(renderer, tex2, nullptr, &dst2);
        SDL_FreeSurface(surf2);
        SDL_DestroyTexture(tex2);

        y += 100;
    }


    SDL_RenderPresent(renderer);
}




void Engine::handleLevelUpEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx = e.button.x;
        int my = e.button.y;

        int y = 150;
        for (int i = 0; i < currentChoices.size(); i++) {
            SDL_Rect box = {150, y, 500, 80};
            if (mx >= box.x && mx <= box.x + box.w &&
                my >= box.y && my <= box.y + box.h) {
                
                player.applySkill(currentChoices[i]);
                state = GameState::RUNNING; 
            }
            y += 100;
        }
    }
}
