#include "include/logic/player.h"
#include <iostream>
#include <cmath>

Player::Player(int startX, int startY)
    : x(startX), y(startY), hp(3), score(0) {}


void Player::move(int dx, int dy, int worldWidth, int worldHeight) {
    x += static_cast<int>(dx * moveSpeed);
    y += static_cast<int>(dy * moveSpeed);
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
    while (exp >= expToNextLevel(level)) { 
        exp -= expToNextLevel(level);
        level++;
    }
}

int Player::expToNextLevel(int currentLevel) const {
    int base = 100;
    double growth = 1.5;
    return static_cast<int>(base * currentLevel * growth);
}


bool Player::checkCollision(Bullet& bullet) {
    float playerLeft = x;
    float playerRight = x + width; 
    float playerTop = y;
    float playerBottom = y + height; 

    float bulletLeft = bullet.x;
    float bulletRight = bullet.x + bullet.width;
    float bulletTop = bullet.y;
    float bulletBottom = bullet.y + bullet.height;

    return (playerLeft < bulletRight && playerRight > bulletLeft &&
            playerTop < bulletBottom && playerBottom > bulletTop);
}



void Player::applySkill(const PlayerSkill& skill) {
    unlockedSkills.push_back(skill);

    switch(skill.type) {
        case PlayerSkillType::INCREASE_BULLET_DAMAGE:
            bulletDamage += 25;
            break;
        case PlayerSkillType::INCREASE_BULLET_SPEED:
            bulletSpeed += 2.0f;
            break;
        case PlayerSkillType::INCREASE_MOVEMENT_SPEED:
            moveSpeed += 1;
            break;
        case PlayerSkillType::EXTRA_HP:
            hp += 1;
            break;
        case PlayerSkillType::RAPID_FIRE:
            fireRateModifier *= 0.8f; 
            break;
        case PlayerSkillType::BIGGER_BULLETS:
            bulletSize += 10;
            break;
        case PlayerSkillType::MULTIPLE_SHOTS:
            extraShots += 1; 
            break;
        case PlayerSkillType::SHOTGUN_SHOTS:
            shotgun = true; 
            break;
        case PlayerSkillType::DIAGONAL_SHOTS:
            diagonalShots = true; 
            break;
        case PlayerSkillType::LASER:
            hasLaser = true;   
            break;

 
        case PlayerSkillType::SMALLER_BODY:
            if (width > 20 && height > 20) {
                width -= 25;
                height -= 25;

            }
            break;
    }

}


std::vector<PlayerSkill> Player::getRandomSkillChoices() {
    std::vector<PlayerSkill> allSkills = {
        {PlayerSkillType::INCREASE_BULLET_DAMAGE, "Power Shot", "Bullets deal more damage"},
        {PlayerSkillType::INCREASE_BULLET_SPEED, "Fast Bullets", "Bullets travel faster"},
        {PlayerSkillType::INCREASE_MOVEMENT_SPEED, "Speed Boost", "Move faster"},
        {PlayerSkillType::EXTRA_HP, "Extra Life", "Gain more HP"},
        {PlayerSkillType::RAPID_FIRE, "Rapid Fire", "Shoot faster"},
        {PlayerSkillType::BIGGER_BULLETS, "Heavy Bullets", "Bullets are bigger"},
        {PlayerSkillType::MULTIPLE_SHOTS, "Double Shot", "Fire extra bullet"},
        {PlayerSkillType::SHOTGUN_SHOTS, "Shotgun", "Fire bullets in spread"},
        {PlayerSkillType::DIAGONAL_SHOTS, "Diagonal Shots", "Adds diagonal bullets"},
        {PlayerSkillType::SMALLER_BODY, "Small Frame", "Reduce player size"},
        {PlayerSkillType::LASER, "Laser Beam", "Shoot a piercing laser"},

    };

    std::vector<PlayerSkill> availableSkills;
    for (auto& skill : allSkills) {
        bool unlocked = false;
        for (auto& s : unlockedSkills) if (s.type == skill.type) unlocked = true;
        if (!unlocked) availableSkills.push_back(skill);
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<PlayerSkill> choices;
    while (choices.size() < 3 && !availableSkills.empty()) {
        int idx = rand() % availableSkills.size();
        choices.push_back(availableSkills[idx]);
        availableSkills.erase(availableSkills.begin() + idx); 
    }
    return choices;
}


std::vector<Bullet> Player::shoot(int targetX, int targetY) {
    std::vector<Bullet> shots;

    float centerX = x + width / 2.0f;
    float centerY = y + height / 2.0f;
    float dx = targetX - centerX;
    float dy = targetY - centerY;
    float length = sqrt(dx * dx + dy * dy);
    if (length == 0) return shots;
    float vx = dx / length * bulletSpeed;
    float vy = dy / length * bulletSpeed;

    BulletType primaryType = BulletType::NORMAL;
    if (hasLaser) {
        primaryType = BulletType::PIERCING;
    }

    shots.emplace_back(centerX, centerY, vx, vy, bulletDamage, bulletSize, primaryType);

    float spreadAngle = 0.15f; 
    for (int i = 1; i <= extraShots; ++i) {
        float angle = spreadAngle * i;
        float cosA = cos(angle);
        float sinA = sin(angle);
        float new_vx = vx * cosA - vy * sinA;
        float new_vy = vx * sinA + vy * cosA;
        
        shots.emplace_back(centerX, centerY, new_vx, new_vy, bulletDamage, bulletSize, primaryType);
    }

    if (diagonalShots) {
        shots.emplace_back(centerX, centerY, vx, -vy, bulletDamage, bulletSize, primaryType);
        shots.emplace_back(centerX, centerY, -vx, vy, bulletDamage, bulletSize, primaryType);
    }

    if (shotgun) {
        float spread = 0.3f;
        float cosA = cos(spread), sinA = sin(spread);
        float vx1 = vx * cosA - vy * sinA;
        float vy1 = vx * sinA + vy * cosA;
        float vx2 = vx * cosA + vy * sinA;
        float vy2 = -vx * sinA + vy * cosA;

        shots.emplace_back(centerX, centerY, vx1, vy1, bulletDamage, bulletSize, primaryType);
        shots.emplace_back(centerX, centerY, vx2, vy2, bulletDamage, bulletSize, primaryType);
    }

    return shots;
}