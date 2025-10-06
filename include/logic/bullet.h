#ifndef BULLET_H
#define BULLET_H

class Bullet {
public:
    float x, y;
    float vx, vy;
    int damage;
    bool alive;
    bool isEnemy;
    int width = 15;
    int height = 15;

    bool piercing = false; 
    bool isGrenade = false; 

    Bullet(float startX, float startY, float velocityX, float velocityY,
           int dmg = 10, int size = 5, bool enemyBullet = false,
           bool piercingBullet = false, bool grenade = false);

    void update();
    void kill();
};

#endif // BULLET_H
