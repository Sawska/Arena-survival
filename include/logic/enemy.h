#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
    public:
    int x,y;
    int hp;
    int damage;
    bool alive = true;
    Enemy(int startX=200, int startY=200, int health=50, int dmg=10);
    void moveTowards(int targetX, int targetY);
    void takeDamage(int damage);
    bool hitPlayer(int playerX, int playerY);
    void die();
};

#endif 