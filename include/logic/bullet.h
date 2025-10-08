#ifndef BULLET_H
#define BULLET_H

enum class BulletType {
    NORMAL,
    PIERCING,
    GRENADE
};

class Bullet {
public:
    float x, y;
    float vx, vy;
    int damage;
    int width, height;
    bool alive = true;
    bool isEnemy = false;
    BulletType type = BulletType::NORMAL;


    float lifetime = 5.0f;     
    float explosionRadius = 0.0f; 
    int piercesLeft = 1;       


    Bullet(float startX, float startY, float velocityX, float velocityY,
           int dmg, int size, BulletType bulletType = BulletType::NORMAL);

    void update();
    void onHit();                
    void kill();
};

#endif // BULLET_H