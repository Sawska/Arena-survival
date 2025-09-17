#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle {
public:
    int x, y;       
    int width, height;

    Obstacle(int x, int y, int width, int height);


    bool collidesWith(int px, int py, int pwidth, int pheight) const;
};

#endif
