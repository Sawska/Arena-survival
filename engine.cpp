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

if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cout << "SDL_mixer could not initialize! Error: "
              << Mix_GetError() << std::endl;
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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
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

    pauseButtons.push_back(Button(300, 200, 200, 50, "Resume", {0,255,0}, {50,255,50}, font));
    pauseButtons.push_back(Button(300, 300, 200, 50, "Exit", {255,0,0}, {255,50,50}, font));



    running = true;
    return true;
}
void Engine::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
            return;
        }

        int mouseX = 0, mouseY = 0;
        bool mousePressed = false;

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            mouseX = event.button.x;
            mouseY = event.button.y;
            mousePressed = true;
        } else if (event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON_LMASK)) {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            mousePressed = true;
        }

        switch (state) {
            case GameState::MENU:
                if (mousePressed) {
                    for (auto& btn : menuButtons) {
                        if (btn.isHovered(mouseX, mouseY)) {
                            if (btn.getText() == "Start") state = GameState::RUNNING;
                            else if (btn.getText() == "Exit") running = false;
                        }
                    }
                }
                break;

            case GameState::GAME_OVER:
                if (mousePressed) {
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
                break;

            case GameState::RUNNING:
                if (mousePressed && event.type == SDL_MOUSEBUTTONDOWN) {
                    int worldMouseX = mouseX + camera.x;
                    int worldMouseY = mouseY + camera.y;
                    auto newBullets = player.shoot(worldMouseX, worldMouseY);
                    if (!newBullets.empty()) playSound("shoot", 0);
                    bullets.insert(bullets.end(), newBullets.begin(), newBullets.end());
                }
                break;

            case GameState::LEVEL_UP:
                handleLevelUpEvent(event);
                break;

            case GameState::PAUSE:
                if (mousePressed) {
                    if (mouseY >= 400 && mouseY <= 420 && mouseX >= 150 && mouseX <= 650) {
                        musicVolume = ((mouseX - 150) * MIX_MAX_VOLUME) / 500;
                        if (musicVolume < 0) musicVolume = 0;
                        if (musicVolume > MIX_MAX_VOLUME) musicVolume = MIX_MAX_VOLUME;
                        Mix_VolumeMusic(musicVolume);
                    }

                    if (mouseY >= 480 && mouseY <= 500 && mouseX >= 150 && mouseX <= 650) {
                        soundVolume = ((mouseX - 150) * MIX_MAX_VOLUME) / 500;
                        if (soundVolume < 0) soundVolume = 0;
                        if (soundVolume > MIX_MAX_VOLUME) soundVolume = MIX_MAX_VOLUME;
                    
                        for (auto& [name, chunk] : soundEffects) {
                            if (chunk) Mix_VolumeChunk(chunk, soundVolume);
                        }

                    }

  
                    if (event.type == SDL_MOUSEBUTTONDOWN) {
                        for (auto& btn : pauseButtons) {
                            if (btn.isHovered(mouseX, mouseY)) {
                                if (btn.getText() == "Resume") state = GameState::RUNNING;
                                else if (btn.getText() == "Exit") running = false;
                            }
                        }
                    }
                }
                break;
        } 
    } 



const Uint8* keystate = SDL_GetKeyboardState(NULL);
if (keystate[SDL_SCANCODE_ESCAPE]) {
    if (!escapePressedLastFrame) {
        if (state == GameState::RUNNING) state = GameState::PAUSE;
        else if (state == GameState::PAUSE) state = GameState::RUNNING;
    }
    escapePressedLastFrame = true;
} else {
    escapePressedLastFrame = false; 
}




    if (state == GameState::RUNNING) {
        int oldX = player.x;
        int oldY = player.y;

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
    }
}



void Engine::update() {
    if(state != GameState::RUNNING) return;

    spawner.update(elapsedTime);

    updateCamera();

    for (auto& enemy : enemies) {
        enemy->moveTowards(player.x, player.y);
        if (enemy->hitPlayer(player)) {
            playSound("hit", 0);
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
            playSound("enemy_death", 0);
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

    
        renderHUD();

    if(state == GameState::PAUSE) {
        renderPauseMenu();
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

        if(state == GameState::RUNNING) {
                elapsedTime += delta; 
        }

        

        switch (state) {
            case GameState::MENU:
                playMusic("menu", -1);
                renderMenu();  
                break;
            case GameState::RUNNING:
                playMusic("background", -1);
                update();
                render();
                break;
            case GameState::GAME_OVER:
                playMusic("game_over_music", -1);
                playSound("game_over", 0);
                renderGameOver();
                break;
            case GameState::LEVEL_UP:
            playSound("level_up", 0);
            render();
            case GameState::PAUSE:
                render();
                break;
        }

        cleanupAudio();

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

void Engine::renderHUD() {
    SDL_Color white = {255, 255, 255, 255};

    int hearts = player.hp / 20; 
    std::string hpText = "HP: ";
    for (int i = 0; i < hearts; i++) {
        hpText += "@ "; 
    }

    int seconds = static_cast<int>(elapsedTime / 1000.0f);
    std::string timeText = "Time: " + std::to_string(seconds) + "s";
    std::string scoreText = "Score: " + std::to_string(player.score);

    int yOffset = 20;

    auto renderText = [&](const std::string& text, int y) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), white);
        if (!surf) {
            std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        if (!tex) {
            std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surf);
            return;
        }

        int screenW, screenH;
        SDL_GetRendererOutputSize(renderer, &screenW, &screenH);

        SDL_Rect dst;
        dst.w = surf->w;
        dst.h = surf->h;
        dst.x = screenW - dst.w - 20; 
        dst.y = y;

        SDL_RenderCopy(renderer, tex, nullptr, &dst);

        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    };

    renderText(hpText, yOffset);
    renderText(timeText, yOffset + 30);
    renderText(scoreText, yOffset + 60);
}


void Engine::renderPauseMenu() {
    SDL_Color white = {255, 255, 255, 255};

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &overlay);

    if (font) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, "Paused", white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst = {250, 50, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);

        int seconds = static_cast<int>(elapsedTime / 1000);
        std::string infoText = "Level: " + std::to_string(player.level) +
                               " | Score: " + std::to_string(player.score) +
                               " | Time: " + std::to_string(seconds) + "s";

        SDL_Surface* surf2 = TTF_RenderText_Solid(font, infoText.c_str(), white);
        SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);
        SDL_Rect dst2 = {150, 120, surf2->w, surf2->h};
        SDL_RenderCopy(renderer, tex2, nullptr, &dst2);
        SDL_FreeSurface(surf2);
        SDL_DestroyTexture(tex2);
    }

    int y = 200;
    for (auto& btn : pauseButtons) {
        SDL_Rect box = {150, y, 500, 80};
        SDL_SetRenderDrawColor(renderer, 50, 50, 100, 255);
        SDL_RenderFillRect(renderer, &box);

        if (font) {
            SDL_Surface* surf = TTF_RenderText_Solid(font, btn.getText().c_str(), white);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_Rect dst = {box.x + 20, box.y + 20, surf->w, surf->h};
            SDL_RenderCopy(renderer, tex, nullptr, &dst);
            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
        }


        y += 100;
    }


SDL_Rect musicBar = {150, 400, 500, 20};   
SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
SDL_RenderFillRect(renderer, &musicBar);


int musicFillWidth = (musicVolume * musicBar.w) / MIX_MAX_VOLUME;
SDL_Rect musicFill = {musicBar.x, musicBar.y, musicFillWidth, musicBar.h};
SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);
SDL_RenderFillRect(renderer, &musicFill);

SDL_Surface* surfMusic = TTF_RenderText_Solid(font, "Music Volume", white);
SDL_Texture* texMusic = SDL_CreateTextureFromSurface(renderer, surfMusic);
SDL_Rect dstMusic = {musicBar.x, musicBar.y - 30, surfMusic->w, surfMusic->h};
SDL_RenderCopy(renderer, texMusic, nullptr, &dstMusic);
SDL_FreeSurface(surfMusic);
SDL_DestroyTexture(texMusic);

SDL_Rect soundBar = {150, 480, 500, 20};
SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
SDL_RenderFillRect(renderer, &soundBar);

int soundFillWidth = (soundVolume * soundBar.w) / MIX_MAX_VOLUME;
SDL_Rect soundFill = {soundBar.x, soundBar.y, soundFillWidth, soundBar.h};
SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
SDL_RenderFillRect(renderer, &soundFill);

SDL_Surface* surfSound = TTF_RenderText_Solid(font, "Sound Volume", white);
SDL_Texture* texSound = SDL_CreateTextureFromSurface(renderer, surfSound);
SDL_Rect dstSound = {soundBar.x, soundBar.y - 30, surfSound->w, surfSound->h};
SDL_RenderCopy(renderer, texSound, nullptr, &dstSound);
SDL_FreeSurface(surfSound);
SDL_DestroyTexture(texSound);


    SDL_RenderPresent(renderer);
}


void Engine::loadAudio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: "
                  << Mix_GetError() << std::endl;
        return;
    }

    Mix_Music* bgMusic = Mix_LoadMUS("assets/audio/background_music.mp3");
    if (!bgMusic) {
        std::cerr << "Failed to load background music! Error: "
                  << Mix_GetError() << std::endl;
    } else {
        musicTracks["background"] = bgMusic;
    }

    Mix_Music* menuMusic = Mix_LoadMUS("assets/audio/menu_music.mp3");
    if (!menuMusic) {
        std::cerr << "Failed to load menu music! Error: "
                  << Mix_GetError() << std::endl;
    } else {
        musicTracks["menu"] = menuMusic;
    }

    Mix_Music* gameOverMusic = Mix_LoadMUS("assets/audio/game_over_music.mp3");
    if (!gameOverMusic) {
        std::cerr << "Failed to load game over music! Error: "
                  << Mix_GetError() << std::endl;
    } else {
        musicTracks["game_over_music"] = gameOverMusic;
    }

    Mix_Chunk* shoot = Mix_LoadWAV("assets/audio/shoot.wav");
    if (!shoot) {
        std::cerr << "Failed to load shoot sound! Error: "
                  << Mix_GetError() << std::endl;
    } else {
        soundEffects["shoot"] = shoot;
    }

    Mix_Chunk* hit = Mix_LoadWAV("assets/audio/hit.wav");
    if (!hit) {
        std::cerr << "Failed to load hit sound! Error: "
                  << Mix_GetError() << std::endl;
    } else {
        soundEffects["hit"] = hit;
    }

    Mix_Chunk* enemyDeath = Mix_LoadWAV("assets/audio/enemy_death.wav");
    if (!enemyDeath) {
        std::cerr << "Failed to load enemy death sound! Error: "
                  << Mix_GetError() << std::endl;
    } else {
        soundEffects["enemy_death"] = enemyDeath;
    }

    Mix_Chunk* gameOverSound = Mix_LoadWAV("assets/audio/game_over.wav");
    if (!gameOverSound) {
        std::cerr << "Failed to load game over sound! Error: "
                  << Mix_GetError() << std::endl;
    } else {
        soundEffects["game_over"] = gameOverSound;
    }

    Mix_Chunk* levelUp = Mix_LoadWAV("assets/audio/level_up.wav");
    if (!levelUp) {
        std::cerr << "Failed to load level up sound! Error: "
                  << Mix_GetError() << std::endl;
    } else {
        soundEffects["level_up"] = levelUp;
    }
}


void Engine::playMusic(const std::string& id, int loops) {
    auto it = musicTracks.find(id);
    if (it != musicTracks.end()) {
        Mix_PlayMusic(it->second, loops);
    }
}

void Engine::playSound(const std::string& id, int loops) {
    auto it = soundEffects.find(id);
    if (it != soundEffects.end()) {
        Mix_PlayChannel(-1, it->second, loops);
    }
}

void Engine::cleanupAudio() {
    for (auto& [id, mus] : musicTracks) {
        Mix_FreeMusic(mus);
    }
    musicTracks.clear();

    for (auto& [id, snd] : soundEffects) {
        Mix_FreeChunk(snd);
    }
    soundEffects.clear();

    Mix_CloseAudio();
}
