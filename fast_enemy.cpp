#include "logic/fast_enemy.h"

FastEnemy::FastEnemy(int startX, int startY) 
    : Enemy(startX, startY, 10, 5) {}

void FastEnemy::moveTowards(int playerX, int playerY) {
    if (x < playerX) x += 3; else x -= 3;
    if (y < playerY) y += 3; else y -= 3;
}

