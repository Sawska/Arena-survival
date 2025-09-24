#include "logic/spawner.h"
#include <cstdlib> 
#include <ctime>   
#include <iostream>

Spawner::Spawner(std::vector<std::unique_ptr<Enemy>>& enemyList,
                 std::vector<Bullet>* enemyBulletsVector,
                 int interval)
    : enemies(enemyList), enemyBullets(enemyBulletsVector),
      spawnInterval(interval), frameCount(0)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}


void Spawner::update(Uint32 elapsedTime, int playerX, int playerY, int screenWidth, int screenHeight) {
    frameCount++;

    int currentSpawnInterval = spawnInterval;
    if (elapsedTime > 300000) currentSpawnInterval = spawnInterval / 4; 
    else if (elapsedTime > 180000) currentSpawnInterval = spawnInterval / 2;
    else if (elapsedTime > 60000) currentSpawnInterval = spawnInterval * 3 / 4;

    if (frameCount >= currentSpawnInterval) {
        frameCount = 0;
        int spawnCount = 1;
        if (elapsedTime > 300000) spawnCount = 3; 
        else if (elapsedTime > 180000) spawnCount = 2;

        for (int i = 0; i < spawnCount; i++) {
            int x, y;
            int edge = std::rand() % 4;
            int offset = 20 + (std::rand() % 50); 

            switch (edge) {
                case 0: 
                    x = playerX + (std::rand() % screenWidth) - screenWidth / 2;
                    y = playerY - screenHeight / 2 - offset;
                    break;
                case 1: 
                    x = playerX + (std::rand() % screenWidth) - screenWidth / 2;
                    y = playerY + screenHeight / 2 + offset;
                    break;
                case 2:
                    x = playerX - screenWidth / 2 - offset;
                    y = playerY + (std::rand() % screenHeight) - screenHeight / 2;
                    break;
                case 3: 
                    x = playerX + screenWidth / 2 + offset;
                    y = playerY + (std::rand() % screenHeight) - screenHeight / 2;
                    break;
            }

            int roll = std::rand() % 100;

            if (elapsedTime < 60000) { 
                enemies.push_back(std::make_unique<Enemy>(x, y, 50, 10));
            } else if (elapsedTime < 180000) { 
                if (roll < 70) enemies.push_back(std::make_unique<Enemy>(x, y, 50, 10));
                else enemies.push_back(std::make_unique<FastEnemy>(x, y));
            } else if (elapsedTime < 300000) { 
                if (roll < 50) enemies.push_back(std::make_unique<Enemy>(x, y, 50, 10));
                else if (roll < 80) enemies.push_back(std::make_unique<FastEnemy>(x, y));
                else enemies.push_back(std::make_unique<TankEnemy>(x, y));
            } else { 
                if (roll < 40) enemies.push_back(std::make_unique<Enemy>(x, y, 50, 10));
                else if (roll < 70) enemies.push_back(std::make_unique<FastEnemy>(x, y));
                else if (roll < 90) enemies.push_back(std::make_unique<TankEnemy>(x, y));
                else enemies.push_back(std::make_unique<RangedEnemy>(x, y, enemyBullets));
            }
        }
    }
}
