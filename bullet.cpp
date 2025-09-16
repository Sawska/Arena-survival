#include "logic/bullet.h"

Bullet::Bullet(float startX, float startY, float velocityX, float velocityY, int dmg, bool enemyBullet)
    : x(startX), y(startY), vx(velocityX), vy(velocityY), damage(dmg), alive(true), isEnemy(enemyBullet)
{}


void Bullet::update() {
    x += vx;
    y += vy;
}

void Bullet::kill() {
    alive = false;
}

