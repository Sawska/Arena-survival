#include "include/logic/player.h"
#include <iostream>

Player::Player(int startX, int startY)
    : x(startX), y(startY), hp(100), score(0) {}

void Player::move(int dx, int dy) {
    x += dx;
    y += dy;
}

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
    if (hp == 0) isAlive = false;
}


void Player::addScore(int points) {
    score += points;


  
        addExp(points);
    
}


bool Player::isDead()
{
    return !isAlive;
}
void Player::addExp(int amount) {
    exp += amount;

    while (true) {
        
        int nextLevelExp = expToNextLevel();  
        
        if (exp >= nextLevelExp) {
            exp -= nextLevelExp;
            level++;            
            std::cout << "Level Up! New level: " << level << std::endl;
        } else {
            break; 
        }
    }
}



int Player::expToNextLevel() const {
    int base = 100;
    double growth = 1.5;
    return static_cast<int>(base * level * growth);
}


bool Player::checkCollision(Bullet& bullet) {
    return x < bullet.x + bullet.width &&
           x + width > bullet.x &&
           y < bullet.y + bullet.height &&
           y + height > bullet.y;
}