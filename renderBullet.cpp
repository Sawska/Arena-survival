#include "logic/renderBullet.h"


void RenderBullet::draw(const Bullet& bullet, SDL_Renderer* renderer, const Camera& cam, int, int, SDL_Texture* playerTex, SDL_Texture* enemyTex,int bulletSize) 
{
    if (bullet.type == BulletType::PIERCING) {
        int centerX = static_cast<int>(bullet.x - cam.x);
        int centerY = static_cast<int>(bullet.y - cam.y);
        
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 200); 
        
        float length = 30.0f;
        float speed = sqrt(bullet.vx * bullet.vx + bullet.vy * bullet.vy);
        if (speed == 0) speed = 1;

        float tailX = centerX - bullet.vx * (length / speed);
        float tailY = centerY - bullet.vy * (length / speed);

        SDL_RenderDrawLine(renderer, centerX - 1, centerY - 1, tailX - 1, tailY - 1);
        SDL_RenderDrawLine(renderer, centerX, centerY, tailX, tailY);
        SDL_RenderDrawLine(renderer, centerX + 1, centerY + 1, tailX + 1, tailY + 1);

    } else {

        int size = (bullet.isEnemy ? 20 : 30) + bulletSize;


        int bx = static_cast<int>(bullet.x - cam.x - size / 2);
        int by = static_cast<int>(bullet.y - cam.y - size / 2);
        

        SDL_Rect rect = { bx, by, size, size };

        SDL_Texture* tex_to_use = bullet.isEnemy ? enemyTex : playerTex;
        if (tex_to_use) {
            SDL_RenderCopy(renderer, tex_to_use, nullptr, &rect);
        }
    }
}
