#ifndef PLAYER_H
#define PLAYER_H

#include "logic/bullet.h"

class Player {
public:
    int x, y;
    int hp;
    int score;
    bool isAlive = true;

    int width = 50;
    int height = 50;

    int level = 1;
    int exp = 0;

    Player(int startX = 100, int startY = 100);

    void move(int dx, int dy);
    void takeDamage(int damage);
    void addScore(int points);

    bool isDead();

    void addExp(int amount);

    
    int expToNextLevel() const;

    bool checkCollision(Bullet& bullet);
    
    
};

#endif
