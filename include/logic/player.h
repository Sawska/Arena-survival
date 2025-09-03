#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    int x, y;
    int hp;
    int score;
    bool isAlive = true;

    Player(int startX = 100, int startY = 100);

    void move(int dx, int dy);
    void takeDamage(int damage);
    void addScore(int points);

    bool isDead(); 
};

#endif
