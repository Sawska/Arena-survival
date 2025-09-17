#include "logic/obstacleSpawner.h"
#include <cstdlib>  
#include <ctime>

std::vector<Obstacle> ObstacleSpawner::spawnRandomObstacles(
    int count, int worldWidth, int worldHeight, int minSize, int maxSize
) {
    std::vector<Obstacle> obstacles;
    obstacles.reserve(count);

    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }


    int actualCount = std::min(count, 8);

    for (int i = 0; i < actualCount; i++) {
        int size = minSize + rand() % (maxSize - minSize + 1);
        int x = rand() % (worldWidth - size);
        int y = rand() % (worldHeight - size);

        obstacles.emplace_back(x, y, size, size);
    }

    return obstacles;
}
