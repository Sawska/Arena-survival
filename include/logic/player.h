#ifndef PLAYER_H
#define PLAYER_H

#include "logic/bullet.h"
#include "playerSkills.h"
#include <vector>
#include <ctime>

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

     float bulletSpeed = 8.0f;
    int bulletDamage = 10;
    int moveSpeed = 5;
    int bulletSize = 5;


    int extraShots = 0;           
    bool shotgun = false;          
    bool diagonalShots = false;    
    float fireRateModifier = 1.0f; 

    std::vector<PlayerSkill> unlockedSkills;

    Player(int startX = 100, int startY = 100);


    void move(int dx, int dy,int worldWidth, int worldHeight);

    void takeDamage(int damage);
    void addScore(int points);

    bool isDead();

    void addExp(int amount);

    
    int expToNextLevel() const;

    bool checkCollision(Bullet& bullet);

    
std::vector<Bullet> shoot(int targetX, int targetY);

    void applySkill(const PlayerSkill& skill);

    std::vector<PlayerSkill> getRandomSkillChoices();
    
    
};

#endif
