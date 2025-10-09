#include "logic/enemy.h"
#include <iostream>



Enemy::Enemy(int startX, int startY, int health, int dmg, float renderScale)
    : x(startX), y(startY), hp(health), damage(1), scale(renderScale)
{}

void Enemy::moveTowards(int targetX, int targetY)
{
    if (x < targetX) x++;
    else if (x > targetX) x--;

    if (y < targetY) y++;
    else if (y > targetY) y--;
}

bool Enemy::hitPlayer(const Player& player) {
    int scaledWidth  = static_cast<int>(width );
    int scaledHeight = static_cast<int>(height);

    if (checkCollision(x, y, scaledWidth, scaledHeight,
                       player.x, player.y, player.width/2, player.height/2)) {
        die();
        return true;
    }
    return false;
}

bool Enemy::hitByBullet(const Bullet& bullet) {
    if (!alive) return false;

    int scaledWidth  = static_cast<int>(width * scale);
    int scaledHeight = static_cast<int>(height * scale);

    bool collisionX = bullet.x + bullet.width > x && bullet.x < x + scaledWidth;
    bool collisionY = bullet.y + bullet.height > y && bullet.y < y + scaledHeight;

    return collisionX && collisionY;
}

void Enemy::takeDamage(int damage) {
    hp -= damage;
    if (hp <= 0) {
        die();
        hp = 0;
    }

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

