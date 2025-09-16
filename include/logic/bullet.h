#ifndef BULLET_H
#define BULLET_H

class Bullet {
public:
    float x, y;
    float vx, vy;
    int damage;
    bool alive;
    bool isEnemy = false; // new flag
    int width = 5;
    int height = 5;

    Bullet(float startX, float startY, float velocityX, float velocityY, int dmg = 10, bool enemyBullet = false);

    void update();
    void kill();
};


#endif // BULLET_H
