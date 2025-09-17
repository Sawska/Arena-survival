#include "include/logic/player.h"
#include <iostream>

Player::Player(int startX, int startY)
    : x(startX), y(startY), hp(100), score(0) {}

void Player::move(int dx, int dy,int worldWidth, int worldHeight) {
    x += dx;
    y += dy;

    if (x < 0) x += worldWidth;
    if (x >= worldWidth) x -= worldWidth;
    if (y < 0) y += worldHeight;
    if (y >= worldHeight) y -= worldHeight;
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



void Player::applySkill(const PlayerSkill& skill) {
    unlockedSkills.push_back(skill);

    switch(skill.type) {
        case PlayerSkillType::INCREASE_BULLET_DAMAGE:
            bulletDamage += 5;
            break;
        case PlayerSkillType::INCREASE_BULLET_SPEED:
            bulletSpeed += 2.0f;
            break;
        case PlayerSkillType::INCREASE_MOVEMENT_SPEED:
            moveSpeed += 1;
            break;
        case PlayerSkillType::EXTRA_HP:
            hp += 20;
            break;
        case PlayerSkillType::RAPID_FIRE:
            fireRateModifier *= 0.8f; 
            break;
        case PlayerSkillType::BIGGER_BULLETS:
            bulletSize += 2;
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
        case PlayerSkillType::SMALLER_BODY:
            if (width > 20 && height > 20) {
                width -= 5;
                height -= 5;
            }
            break;
    }

    std::cout << "Skill gained: " << skill.name << std::endl;
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
        {PlayerSkillType::SMALLER_BODY, "Small Frame", "Reduce player size"}
    };

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<PlayerSkill> choices;
    while(choices.size() < 3) {
        int idx = rand() % allSkills.size();
        bool already = false;
        for(const auto& c : choices) if(c.type == allSkills[idx].type) already = true;
        if(!already) choices.push_back(allSkills[idx]);
    }
    return choices;
}
