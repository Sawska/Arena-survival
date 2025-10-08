#include "logic/bullet.h"

Bullet::Bullet(float startX, float startY, float velocityX, float velocityY,
               int dmg, int size, BulletType bulletType)
    : x(startX), y(startY),
      vx(velocityX), vy(velocityY),
      damage(dmg),
      width(size), height(size),
      type(bulletType)
{
    switch (type) {
        case BulletType::PIERCING:
            piercesLeft = 5; 
            lifetime = 1.0f; 
            break;
        case BulletType::GRENADE:
            damage /= 2; 
            explosionRadius = 75.0f;
            lifetime = 2.0f;     
            vx *= 0.6f;           
            vy *= 0.6f;
            break;
        case BulletType::NORMAL:

            piercesLeft = 1;
            break;
    }
}

void Bullet::update() {
    x += vx ;
    y += vy ;
}


void Bullet::kill() {
    alive = false;

}
