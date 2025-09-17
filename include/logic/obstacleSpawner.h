#ifndef OBSTACLESPAWNER_H
#define OBSTACLESPAWNER_H

#include <vector>
#include "Obstacle.h"

class ObstacleSpawner {
public:
    static std::vector<Obstacle> spawnRandomObstacles(
        int count, int worldWidth, int worldHeight, int minSize, int maxSize
    );
};

#endif
