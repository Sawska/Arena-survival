#ifndef TANK_ENEMY_H
#define TANK_ENEMY_H

#include "enemy.h"

class TankEnemy : public Enemy {
public:
    TankEnemy(int startX, int startY);
    void moveTowards(int playerX, int playerY) override;
};

#endif
