#include "logic/tank_enemy.h"

TankEnemy::TankEnemy(int startX, int startY)
    : Enemy(startX, startY, 200, 80,5.0f) {} 




void TankEnemy::moveTowards(int playerX, int playerY) {
    if (x < playerX) x += 1; else x -= 1;
    if (y < playerY) y += 1; else y -= 1;
}

