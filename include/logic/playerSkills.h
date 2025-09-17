#ifndef PLAYER_SKILLS_H
#define PLAYER_SKILLS_H

#include <string>

enum class PlayerSkillType {
    INCREASE_BULLET_DAMAGE,
    INCREASE_BULLET_SPEED,
    INCREASE_MOVEMENT_SPEED,
    EXTRA_HP,
    RAPID_FIRE,
    BIGGER_BULLETS,
    MULTIPLE_SHOTS,
    SHOTGUN_SHOTS,
    DIAGONAL_SHOTS,
    SMALLER_BODY
};


struct PlayerSkill {
    PlayerSkillType type;
    std::string name;
    std::string description;
};

#endif
