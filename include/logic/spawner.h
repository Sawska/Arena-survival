#ifndef SPAWNER_H
#define SPAWNER_H

#include "enemy.h"
#include "fast_enemy.h"
#include "tank_enemy.h"
#include "ranged_enemy.h"
#include <vector>
#include <memory>
#include <SDL2/SDL.h>

class Spawner {
    std::vector<std::unique_ptr<Enemy>>& enemies;
    std::vector<Bullet>* enemyBullets; 
    int spawnInterval;
    int frameCount;

public:
    Spawner(std::vector<std::unique_ptr<Enemy>>& enemyList,
            std::vector<Bullet>* enemyBulletsVector,
            int interval = 120);
   void update(Uint32 elapsedTime, int playerX, int playerY, int screenWidth, int screenHeight);
};


#endif 
