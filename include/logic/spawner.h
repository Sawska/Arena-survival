#ifndef SPAWNER_H

#define SPAWNER_H

#include "enemy.h"
#include <vector>

class Spawner {
    std::vector<Enemy>& enemies;
    int spawnInterval; 
    int frameCount;
public:
    Spawner(std::vector<Enemy>& enemyList, int interval = 120);
    void update();
};

#endif 