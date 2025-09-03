#include "player.h"

Player::Player() {
    x = 0;
    y = 0;
}

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

void Player::addScore(int points) {
    score += points;
}

void Player::reset() {
    hp = 100;
    score = 0;
    x = 0;
    y = 0;
    attackPower = 10;
}

Player::~Player() {
 
}