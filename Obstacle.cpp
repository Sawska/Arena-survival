#include "logic/Obstacle.h"

Obstacle::Obstacle(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

bool Obstacle::collidesWith(int px, int py, int pwidth, int pheight) const {
    return !(px + pwidth <= x ||       
             px >= x + width ||        
             py + pheight <= y ||      
             py >= y + height);        
}
