#include "logic/enemy.h"



Enemy::Enemy(int startX, int startY, int health, int dmg)
{
    x = startX;
    y = startY;
    hp = health;
    damage = dmg;
}

void Enemy::moveTowards(int targetX, int targetY)
{
    if (x < targetX) x++;
    else if (x > targetX) x--;

    if (y < targetY) y++;
    else if (y > targetY) y--;
}

bool Enemy::hitPlayer(int playerX, int playerY) {
    if (x == playerX && y == playerY) {
        die();
        return true;
    }
    return false;
}


void Enemy::takeDamage(int damage)
{
    hp -= damage;
    if (hp < 0) hp = 0;
}

void Enemy::die() {
    alive = false;
}
