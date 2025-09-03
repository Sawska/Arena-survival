#ifndef RENDERER_H

#define RENDERER_H

#include <iostream>
#include <SDL2/SDL.h>



class Renderer {
    public: 
    Renderer();
    ~Renderer();

    SDL_Renderer* renderer;
    SDL_Window* window;
    
};

#endif // RENDERER_H