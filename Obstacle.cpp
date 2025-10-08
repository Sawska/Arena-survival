#include "logic/Obstacle.h"

Obstacle::Obstacle(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

bool Obstacle::collidesWith(int px, int py, int pwidth, int pheight) const {
    return !(px + pwidth <= (x+60) ||       
             px >= x + width - 60 ||        
             py + pheight <= (y+60) ||      
             py >= y + height - 60);        
}
