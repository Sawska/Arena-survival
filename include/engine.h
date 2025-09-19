#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "logic/player.h"
#include "render/renderPlayer.h"
#include "logic/enemy.h"
#include "render/renderEnemy.h"
#include "logic/spawner.h"
#include "logic/bullet.h"
#include "logic/renderBullet.h"
#include "logic/camera.h"
#include "logic/Obstacle.h"
#include "render/renderObstacle.h"
#include <vector>
#include "logic/obstacleSpawner.h"
#include "render/button.h"
#include <SDL2/SDL_mixer.h>
#include <map>






enum class GameState {
    MENU,
    RUNNING,
    GAME_OVER,
    LEVEL_UP,
    PAUSE
};





class Engine {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Player player;
    RenderPlayer renderPlayer;

    std::vector<std::unique_ptr<Enemy>> enemies;   
     Spawner spawner;             
    RenderEnemy renderEnemy;
    
    std::vector<Bullet> bullets;
    RenderBullet renderBullet;

    std::vector<Bullet> enemyBullets;

    std::vector<Obstacle> obstacles;
    RenderObstacle renderObstacle;
    ObstacleSpawner obstacleSpawner;

    bool escapePressedLastFrame = false;



    Uint32 elapsedTime = 0;
    Uint32 finalElapsedTime = 0;

    int lastPlayerLevel = 1;

    std::map<std::string, Mix_Music*> musicTracks;
    std::map<std::string, Mix_Chunk*> soundEffects;
    


    void updateCamera();

    TTF_Font* font = nullptr;
    std::vector<Button> menuButtons;
    std::vector<Button> gameOverButtons;

    std::vector<Button> pauseButtons;


    std::vector<PlayerSkill> currentChoices;

    int musicVolume = MIX_MAX_VOLUME;  
int soundVolume = MIX_MAX_VOLUME;  

    
        void triggerLevelUp();
    void renderLevelUpScreen();
    void handleLevelUpEvent(SDL_Event& e);


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
    void renderMenu();
    void renderHUD();
    void renderPauseMenu();
    void loadAudio();
    void playMusic(const std::string& id, int loops = -1);
    void playSound(const std::string& id, int loops = 0);
    void cleanupAudio();
     GameState state; 
    Camera camera;
    int worldWidth = 1600;
    int worldHeight = 1200;
};




#endif
