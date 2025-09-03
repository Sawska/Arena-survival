#include <renderer.h>

Renderer::Renderer() {
     if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error:" << SDL_GetError() << std::endl;
    }

     window = SDL_CreateWindow("Survival arena",100,100,800,600,SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "SDL_CreateWidnow Error:" << SDL_GetError() << std::endl;
        SDL_Quit();
    }

     renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer,0,0,255,255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(2000);

}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

