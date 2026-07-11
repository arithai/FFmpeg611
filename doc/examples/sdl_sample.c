#include <SDL.h>

int main(int argc, char *argv[]) {
    int running;
    SDL_Window *window;
    SDL_Surface *windowsurface;
    SDL_Surface *image;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Hello World",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              592, 460, SDL_WINDOW_SHOWN);
    windowsurface = SDL_GetWindowSurface(window);

    image = SDL_LoadBMP("exampleimage.bmp");
    SDL_BlitSurface(image, NULL, windowsurface, NULL);

    SDL_UpdateWindowSurface(window);
    SDL_FreeSurface(image);

    running = 1;
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = 0;
                break;
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
 }
