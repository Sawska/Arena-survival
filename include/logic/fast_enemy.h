#ifndef FAST_ENEMY_H
#define FAST_ENEMY_H

#include "enemy.h"

class FastEnemy : public Enemy {
public:
    FastEnemy(int startX, int startY);
    void moveTowards(int playerX, int playerY) override;

    int getScoreValue() const override { return 15; }
};

#endif
