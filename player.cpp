#include "include/logic/player.h"

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
}

bool Player::isDead()
{
    return !isAlive;
}
