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



if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    SDL_Log("SDL_image init failed: %s", IMG_GetError());
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
        font = TTF_OpenFont("assets/kenney_ui-pack/Font/Kenney Future Narrow.ttf", 24);
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


SDL_Surface* bgSurface = IMG_Load("assets/background.png");
if (!bgSurface) {
    std::cout << "Failed to load background image: " << IMG_GetError() << std::endl;
    return false;
}

background = SDL_CreateTextureFromSurface(renderer, bgSurface);
SDL_FreeSurface(bgSurface);

if (!background) {
    std::cout << "Failed to create background texture: " << SDL_GetError() << std::endl;
    return false;
}




loadGUITextures();
renderEnemy.loadTextures(renderer);
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
            if(!menuMusicPlaying) {
    playMusic("menu_music", -1);
    menuMusicPlaying = true; 
            }
                if (mousePressed) {
                    for (auto& btn : menuButtons) {
                        if (btn.isHovered(mouseX, mouseY)) {
                            if (btn.getText() == "Start") state = GameState::RUNNING;
                            else if (btn.getText() == "Exit") running = false, menuMusicPlaying = false;
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
                                loadedChunks.clear();
                                generatedChunks.clear();
                                obstacles.clear();
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
        SDL_Rect musicBar = {150 + 20, 100 + 300, 500 - 40, 20};
        if (mouseY >= musicBar.y && mouseY <= musicBar.y + musicBar.h &&
            mouseX >= musicBar.x && mouseX <= musicBar.x + musicBar.w) {
            musicVolume = ((mouseX - musicBar.x) * MIX_MAX_VOLUME) / musicBar.w;
            musicVolume = std::clamp(musicVolume, 0, MIX_MAX_VOLUME);
            Mix_VolumeMusic(musicVolume);
        }

        SDL_Rect soundBar = {150 + 20, 100 + 340, 500 - 40, 20};
        if (mouseY >= soundBar.y && mouseY <= soundBar.y + soundBar.h &&
            mouseX >= soundBar.x && mouseX <= soundBar.x + soundBar.w) {
            soundVolume = ((mouseX - soundBar.x) * MIX_MAX_VOLUME) / soundBar.w;
            soundVolume = std::clamp(soundVolume, 0, MIX_MAX_VOLUME);

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



playerChunkX = player.x / chunkSize;
playerChunkY = player.y / chunkSize;

ChunkCoord currentChunk{playerChunkX, playerChunkY};


if (generatedChunks.find(currentChunk) == generatedChunks.end()) {
    generatedChunks[currentChunk] = obstacleSpawner.spawnRandomObstacles(10, chunkSize, chunkSize, 50, 150);

    for (auto& obs : generatedChunks[currentChunk]) {
        obs.x += playerChunkX * chunkSize;
        obs.y += playerChunkY * chunkSize;
    }
}


    spawner.update(elapsedTime, player.x, player.y, 800, 600);

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

            if (bullet.isGrenade) {
                float explosionRadius = 100.0f;
                for (auto& aoeEnemy : enemies) {
                    float dx = aoeEnemy->x - bullet.x;
                    float dy = aoeEnemy->y - bullet.y;
                    float dist2 = dx*dx + dy*dy;
                    if (aoeEnemy->alive && dist2 < explosionRadius * explosionRadius) {
                        aoeEnemy->takeDamage(bullet.damage / 2); 
                    }
                }
                bullet.kill(); 
                break; 
            }
            else if (!bullet.piercing) {

                bullet.kill();
                break;
            }

        }
    }
}





bullets.erase(
    std::remove_if(bullets.begin(), bullets.end(),
        [this](const Bullet& b) {
            if (!b.alive) return true;
            float dx = b.x - player.x;
            float dy = b.y - player.y;
            return (dx*dx + dy*dy > 500*500);
        }),
    bullets.end()
);

enemyBullets.erase(
    std::remove_if(enemyBullets.begin(), enemyBullets.end(),
        [this](const Bullet& b) {
            if (!b.alive) return true;
            float dx = b.x - player.x;
            float dy = b.y - player.y;
            return (dx*dx + dy*dy > 500*500);
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
            int points = (*it)->getScoreValue();
            player.addScore(points);
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



int renderDistance = 1; 
obstacles.clear();

for (int dx = -renderDistance; dx <= renderDistance; dx++) {
    for (int dy = -renderDistance; dy <= renderDistance; dy++) {
        ChunkCoord neighbor{playerChunkX + dx, playerChunkY + dy};
        if (generatedChunks.count(neighbor)) {
            for (auto& obs : generatedChunks[neighbor]) {
                obstacles.push_back(obs);
            }
        }
    }
}



    SDL_RenderCopy(renderer, background, nullptr, nullptr); 




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

std::ostream& operator<<(std::ostream& os, GameState s) {
    switch (s) {
        case GameState::MENU: return os << "MENU";
        case GameState::RUNNING: return os << "RUNNING";
        case GameState::GAME_OVER: return os << "GAME_OVER";
        case GameState::LEVEL_UP: return os << "LEVEL_UP";
        case GameState::PAUSE: return os << "PAUSE";
        default: return os << "UNKNOWN";
    }
}



void Engine::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::run() {
    if (!init()) return;

     Uint32 lastTime = SDL_GetTicks();

     loadAudio();

     Mix_Volume(-1, MIX_MAX_VOLUME);

std::cout << "State: " << state << std::endl;



   while (running) {
    handleEvents();
    Uint32 currentTime = SDL_GetTicks();
    Uint32 delta = currentTime - lastTime;
    lastTime = currentTime;

    if (state == GameState::RUNNING) {
        elapsedTime += delta;
    }
    if (state != previousState) {
    switch (state) {
        case GameState::MENU:
            playMusic("menu_music", -1);
            break;
        case GameState::RUNNING:
            if (previousState != GameState::PAUSE) { 
                playMusic("background", -1);
            }
            break;
        case GameState::GAME_OVER:
            playMusic("game_over_music", -1);
            playSound("game_over", 0);
            break;
        case GameState::LEVEL_UP:
            if (!levelUpSoundPlayed) {
                playSound("level_up", 0);
                levelUpSoundPlayed = true;
            }
            break;
    }
    previousState = state;
}


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
        case GameState::PAUSE:
            render();
            break;
    }

    SDL_Delay(16);
}

cleanupAudio();

}


void Engine::renderGameOver() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect panel { 100, 100, 600, 400 };
    if (guiTextures.count("panel_gameover") && guiTextures[""]) {
        SDL_RenderCopy(renderer, guiTextures["panel_gameover"], nullptr, &panel);
    } else {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &panel);
    }
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &panel);

    SDL_Color white = {255,255,255,255};
    if (font) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, "Game Over", white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst { panel.x + (panel.w - surf->w) / 2, panel.y + 20, surf->w, surf->h };
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);

        int seconds = finalElapsedTime / 1000;
        std::string infoText = "You survived: " + std::to_string(seconds) + "s | "
                               "\nLevel: " + std::to_string(player.level) + " | "
                               "\nScore: " + std::to_string(player.score);

        int wrapLength = panel.w - 40;
        SDL_Surface* surf2 = TTF_RenderText_Blended_Wrapped(font, infoText.c_str(), white, wrapLength);
        SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);
        SDL_Rect dst2 { panel.x + 20, panel.y + 80, surf2->w, surf2->h };
        SDL_RenderCopy(renderer, tex2, nullptr, &dst2);
        SDL_FreeSurface(surf2);
        SDL_DestroyTexture(tex2);
    }

    int numButtons = static_cast<int>(gameOverButtons.size());
    int btnW = 220;
    int btnH = 60;
    int gap = 16;
    int bottomMargin = 36;

    int totalHeight = numButtons * btnH + (numButtons - 1) * gap;
    int startY = panel.y + panel.h - bottomMargin - totalHeight;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    for (int i = 0; i < numButtons; ++i) {
        Button& btn = gameOverButtons[i];

        SDL_Rect buttonBox;
        buttonBox.w = btnW;
        buttonBox.h = btnH;
        buttonBox.x = panel.x + (panel.w - btnW) / 2; 
        buttonBox.y = startY + i * (btnH + gap);

        btn.rect = buttonBox;


        if (guiTextures.count("button") && guiTextures["button"]) {
            SDL_RenderCopy(renderer, guiTextures["button"], nullptr, &buttonBox);
        } else {
            SDL_SetRenderDrawColor(renderer, 50, 50, 150, 255);
            SDL_RenderFillRect(renderer, &buttonBox);
        }

        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // SDL_RenderDrawRect(renderer, &buttonBox);

        if (font) {
            SDL_Surface* textSurf = TTF_RenderText_Solid(font, btn.getText().c_str(), white);
            SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, textSurf);

            SDL_Rect textDst;
            textDst.w = textSurf->w;
            textDst.h = textSurf->h;
            textDst.x = buttonBox.x + (buttonBox.w - textDst.w) / 2;
            textDst.y = buttonBox.y + (buttonBox.h - textDst.h) / 2;

            SDL_RenderCopy(renderer, textTex, nullptr, &textDst);

            SDL_FreeSurface(textSurf);
            SDL_DestroyTexture(textTex);
        }
    }

    SDL_RenderPresent(renderer);
}




void Engine::renderMenu() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect panel {100, 100, 600, 400};
    if (guiTextures["panel"]) {
        SDL_RenderCopy(renderer, guiTextures["panel"], nullptr, &panel);
    } else {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &panel);
    }

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &panel);

    SDL_Color white = {255, 255, 255, 255};
    if (font) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, "My Game", white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst { panel.x + (panel.w - surf->w)/2, panel.y + 20, surf->w, surf->h };
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    int btnY = panel.y + 100; 
    for (auto& btn : menuButtons) {

        btn.rect.x = panel.x + (panel.w - btn.rect.w)/2;
        btn.rect.y = btnY;


        if (guiTextures["button"]) {
            SDL_RenderCopy(renderer, guiTextures["button"], nullptr, &btn.rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 70, 70, 200, 255);
            SDL_RenderFillRect(renderer, &btn.rect);
        }


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &btn.rect);


        if (font) {
            SDL_Surface* textSurf = TTF_RenderText_Solid(font, btn.text.c_str(), white);
            SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, textSurf);

            SDL_Rect textDst;
            textDst.w = textSurf->w;
            textDst.h = textSurf->h;
            textDst.x = btn.rect.x + (btn.rect.w - textDst.w)/2;
            textDst.y = btn.rect.y + (btn.rect.h - textDst.h)/2;

            SDL_RenderCopy(renderer, textTex, nullptr, &textDst);

            SDL_FreeSurface(textSurf);
            SDL_DestroyTexture(textTex);
        }

        btnY += btn.rect.h + 20; 
    }

    SDL_RenderPresent(renderer);
}






float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}


void Engine::loadGUITextures() {
    guiTextures["panel"] = IMG_LoadTexture(renderer, "assets/kenney_ui-pack/PNG/Blue/Default/button_square_gloss.png");
    guiTextures["panel_pause"] = IMG_LoadTexture(renderer, "assets/kenney_ui-pack/PNG/Greey/Default/button_square_gloss.png");
    guiTextures["panel_gameover"] = IMG_LoadTexture(renderer, "assets/kenney_ui-pack/PNG/Red/Default/button_square_gloss.png");
    guiTextures["button"] = IMG_LoadTexture(renderer, "assets/kenney_ui-pack/PNG/Greey/Default/button_rectangle_gloss.png");
    guiTextures["button_pause"] = IMG_LoadTexture(renderer, "assets/kenney_ui-pack/PNG/Blue/Default/button_rectangle_gloss.png");
    guiTextures["button_gameover"] = IMG_LoadTexture(renderer, "assets/kenney_ui-pack/PNG/Blue/Default/button_rectangle_gloss.png");
    



    for (auto& [key, tex] : guiTextures) {
        if (!tex) {
            std::cerr << "Failed to load GUI texture " << key << ": " << IMG_GetError() << std::endl;
        }
    }
}




void Engine::updateCamera() {


camera.x = lerp(camera.x, player.x - 400, 0.1f);
camera.y = lerp(camera.y, player.y - 300, 0.1f);
 

}


void Engine::triggerLevelUp() {
    state = GameState::LEVEL_UP;
    currentChoices = player.getRandomSkillChoices();
}


void Engine::renderLevelUpScreen() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);  
    SDL_Rect overlay = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &overlay);

    SDL_Rect panel {150, 100, 500, 400};
    if (guiTextures.count("panel") && guiTextures["panel"]) {
        SDL_RenderCopy(renderer, guiTextures["panel"], nullptr, &panel);
    } else {
        SDL_SetRenderDrawColor(renderer, 50, 50, 100, 255);
        SDL_RenderFillRect(renderer, &panel);
    }
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &panel);

    SDL_Color white = {255, 255, 255, 255};

    if (font) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, "Choose a Skill", white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst { panel.x + (panel.w - surf->w)/2, panel.y + 20, surf->w, surf->h };
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }


    int numChoices = static_cast<int>(currentChoices.size());
    int btnW = 400;
    int btnH = 60;
    int gap = 20;
    int startY = panel.y + 80; 

    for (int i = 0; i < numChoices; ++i) {
        auto& skill = currentChoices[i];

        SDL_Rect btnRect;
        btnRect.w = btnW;
        btnRect.h = btnH;
        btnRect.x = panel.x + (panel.w - btnW)/2;
        btnRect.y = startY + i * (btnH + gap);

        if (guiTextures.count("button") && guiTextures["button"]) {
            SDL_RenderCopy(renderer, guiTextures["button"], nullptr, &btnRect);
        } else {
            SDL_SetRenderDrawColor(renderer, 50, 50, 150, 255);
            SDL_RenderFillRect(renderer, &btnRect);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &btnRect);

        if (font) {
            SDL_Surface* textSurf = TTF_RenderText_Solid(font, skill.name.c_str(), white);
            SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, textSurf);

            SDL_Rect textDst;
            textDst.w = textSurf->w;
            textDst.h = textSurf->h;
            textDst.x = btnRect.x + (btnRect.w - textDst.w)/2;
            textDst.y = btnRect.y + (btnRect.h - textDst.h)/2;

            SDL_RenderCopy(renderer, textTex, nullptr, &textDst);

            SDL_FreeSurface(textSurf);
            SDL_DestroyTexture(textTex);
        }
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

    int seconds = static_cast<int>(elapsedTime / 1000.0);
    std::string timeText = "Time: " + std::to_string(seconds) + "Sec";
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

    SDL_Rect panel {150, 100, 500, 400};
    if (guiTextures["panel_pause"]) {
        SDL_RenderCopy(renderer, guiTextures["panel_pause"], nullptr, &panel);
    } else {
        SDL_SetRenderDrawColor(renderer, 50, 50, 100, 255); 
        SDL_RenderFillRect(renderer, &panel);
    }
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); 
    SDL_RenderDrawRect(renderer, &panel);

    if (font) {
        SDL_Surface* surf = TTF_RenderText_Solid(font, "Paused", white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst {panel.x + (panel.w - surf->w)/2, panel.y + 20, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);

        int seconds = static_cast<int>(elapsedTime / 1000);
        std::string infoText = "Level: " + std::to_string(player.level) +
                               " | Score: " + std::to_string(player.score) +
                               " | Time: " + std::to_string(seconds) + "s";

        SDL_Surface* surf2 = TTF_RenderText_Solid(font, infoText.c_str(), white);
        SDL_Texture* tex2 = SDL_CreateTextureFromSurface(renderer, surf2);
        SDL_Rect dst2 {panel.x + 20, panel.y + 60, surf2->w, surf2->h};
        SDL_RenderCopy(renderer, tex2, nullptr, &dst2);
        SDL_FreeSurface(surf2);
        SDL_DestroyTexture(tex2);
    }
    int btnY = panel.y + 120;
    for (auto& btn : pauseButtons) {
        btn.rect.x = panel.x + (panel.w - btn.rect.w)/2;
        btn.rect.y = btnY;

        if (guiTextures["button_pause"]) {
            SDL_RenderCopy(renderer, guiTextures["button_pause"], nullptr, &btn.rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 70, 150, 70, 255);
            SDL_RenderFillRect(renderer, &btn.rect);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
        SDL_RenderDrawRect(renderer, &btn.rect);

        if (font) {
            SDL_Surface* textSurf = TTF_RenderText_Solid(font, btn.text.c_str(), white);
            SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, textSurf);

            SDL_Rect textDst;
            textDst.w = textSurf->w;
            textDst.h = textSurf->h;
            textDst.x = btn.rect.x + (btn.rect.w - textDst.w)/2;
            textDst.y = btn.rect.y + (btn.rect.h - textDst.h)/2;

            SDL_RenderCopy(renderer, textTex, nullptr, &textDst);

            SDL_FreeSurface(textSurf);
            SDL_DestroyTexture(textTex);
        }

        btnY += btn.rect.h + 20;
    }

    SDL_Rect musicBar = {panel.x + 20, panel.y + 300, panel.w - 40, 20};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &musicBar);

    int musicFillWidth = (musicVolume * musicBar.w) / MIX_MAX_VOLUME;
    SDL_Rect musicFill = {musicBar.x, musicBar.y, musicFillWidth, musicBar.h};
    SDL_SetRenderDrawColor(renderer, 0, 200, 50, 255);
    SDL_RenderFillRect(renderer, &musicFill);

    SDL_Surface* surfMusic = TTF_RenderText_Solid(font, "Music Volume", white);
    SDL_Texture* texMusic = SDL_CreateTextureFromSurface(renderer, surfMusic);
    SDL_Rect dstMusic = {musicBar.x, musicBar.y - 25, surfMusic->w, surfMusic->h};
    SDL_RenderCopy(renderer, texMusic, nullptr, &dstMusic);
    SDL_FreeSurface(surfMusic);
    SDL_DestroyTexture(texMusic);

    SDL_Rect soundBar = {panel.x + 20, panel.y + 340, panel.w - 40, 20};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &soundBar);

    int soundFillWidth = (soundVolume * soundBar.w) / MIX_MAX_VOLUME;
    SDL_Rect soundFill = {soundBar.x, soundBar.y, soundFillWidth, soundBar.h};
    SDL_SetRenderDrawColor(renderer, 0, 150, 200, 255); 
    SDL_RenderFillRect(renderer, &soundFill);

    SDL_Surface* surfSound = TTF_RenderText_Solid(font, "Sound Volume", white);
    SDL_Texture* texSound = SDL_CreateTextureFromSurface(renderer, surfSound);
    SDL_Rect dstSound = {soundBar.x, soundBar.y - 25, surfSound->w, surfSound->h};
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
        musicTracks["menu_music"] = menuMusic;
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
    if (Mix_PlayMusic(it->second, loops) == -1) {
        std::cerr << "Mix_PlayMusic failed for " << id 
                  << "! Error: " << Mix_GetError() << std::endl;
    } else {
        std::cout << "Playing music: " << id << std::endl;
    }
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
