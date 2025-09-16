#ifndef RANGED_ENEMY_H
#define RANGED_ENEMY_H

#include "enemy.h"
#include <vector>



class RangedEnemy : public Enemy {
public:
    int cooldown = 0;
    std::vector<Bullet>* enemyBullets; 

    RangedEnemy(int startX, int startY, std::vector<Bullet>* bullets);
    void moveTowards(int playerX, int playerY) override;
};

#endif
