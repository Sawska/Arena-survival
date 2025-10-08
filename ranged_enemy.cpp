#include "logic/ranged_enemy.h"
#include <cmath>

RangedEnemy::RangedEnemy(int startX, int startY, std::vector<Bullet>* bullets)
    : Enemy(startX, startY, 30, 1,3.0f), enemyBullets(bullets) {}

void RangedEnemy::moveTowards(int playerX, int playerY) {

    if (x < playerX - 200) x += 2;
    if (x > playerX + 200) x -= 2;
    if (y < playerY - 200) y += 2;
    if (y > playerY + 200) y -= 2;


    if (cooldown > 0) {
        cooldown--;
    } else {
        float dx = playerX - x;
        float dy = playerY - y;
        float len = std::sqrt(dx*dx + dy*dy);
        if (len != 0) {
            float vx = dx / len * 5.0f;
            float vy = dy / len * 5.0f;
            enemyBullets->push_back(Bullet(x, y, vx, vy, 5, 20, BulletType::NORMAL));
            cooldown = 60; 
        }
    }
}
