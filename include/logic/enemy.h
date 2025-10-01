#ifndef ENEMY_H
#define ENEMY_H

#include "player.h"
#include "logic/bullet.h"

class Enemy {
public:
    int x, y;
    int hp;
    int width = 50;  
    int height = 50;  
    int damage;
    bool alive = true;

    float scale; 

    Enemy(int startX = 200, int startY = 200, int health = 50, int dmg = 10, float renderScale=3.0f);
    virtual ~Enemy() = default;

    virtual void moveTowards(int targetX, int targetY);
    virtual void update(int playerX, int playerY) { moveTowards(playerX, playerY); }

    void takeDamage(int damage);
    void die();
    bool hitPlayer(const Player& player);
    bool checkCollision(int x1, int y1, int w1, int h1,
                        int x2, int y2, int w2, int h2);

    bool hitByBullet(const Bullet& bullet);

    virtual int getScoreValue() const { return 10; }  
};

#endif
