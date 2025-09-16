#include "logic/enemy.h"
#include <iostream>



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

bool Enemy::hitPlayer(const Player& player) {
    if (checkCollision(x, y, width, height,
                       player.x, player.y, player.width, player.height)) {
        die();
        return true;
    }
    return false;
}





void Enemy::takeDamage(int damage)
{
    hp -= damage;
    if (hp < 0) {
        die();
        hp = 0;
    }

    std::cout << "Enemy took " << damage << " damage, HP left: " << hp << std::endl;
}

void Enemy::die() {
    alive = false;
}

bool Enemy::checkCollision(int x1, int y1, int w1, int h1,
                    int x2, int y2, int w2, int h2) {
    return x1 < x2 + w2 &&
           x1 + w1 > x2 &&
           y1 < y2 + h2 &&
           y1 + h1 > y2;
}



bool Enemy::hitByBullet(const Bullet& bullet) {
    if (!alive) return false;

    bool collisionX = bullet.x + bullet.width > x && bullet.x < x + width;
    bool collisionY = bullet.y + bullet.height > y && bullet.y < y + height;

    return collisionX && collisionY;
}

