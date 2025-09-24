#include "logic/bullet.h"

Bullet::Bullet(float startX, float startY, float velocityX, float velocityY,
               int dmg, int size, bool enemyBullet,
               bool piercingBullet, bool grenade)
    : x(startX), y(startY),
      vx(velocityX), vy(velocityY),
      damage(dmg),
      alive(true),
      isEnemy(enemyBullet),
      width(size), height(size),
      piercing(piercingBullet),  
      isGrenade(grenade) {}      


void Bullet::update() {
    x += vx;
    y += vy;
}

void Bullet::kill() {
    alive = false;
}

