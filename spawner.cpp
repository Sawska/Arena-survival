#include "logic/spawner.h"
#include <cstdlib> 
#include <ctime>   
#include <iostream>

Spawner::Spawner(std::vector<Enemy>& enemyList, int interval)
    : enemies(enemyList), spawnInterval(interval), frameCount(0) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 
}
void Spawner::update() {
    frameCount++;
    if (frameCount >= spawnInterval) {
        frameCount = 0;
        int x = std::rand() % 800; 
        int y = std::rand() % 600; 
        enemies.emplace_back(x, y);
        std::cout << "Spawned enemy at (" << x << ", " << y << ")\n";
    }
}

