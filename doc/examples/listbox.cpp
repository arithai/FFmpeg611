#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

const int WIDTH = 400;
const int HEIGHT = 300;

int listbox_main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("SDL Listbox Sample", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Ensure you have a valid .ttf font path available in your working directory
    TTF_Font* font = TTF_OpenFont("m.ttf", 18);
    if (!font) {
        // Handle font load failure
    }

    std::vector<std::string> items = {"Cygwin Item 1", "SDL2 Listbox", "Item Three", "Text Element Four"};
    int selectedIndex = 0;

    SDL_Rect listboxRect = {50, 50, 300, 200};
    int itemHeight = 30;

    bool running = true;
    SDL_Event e;
//listbox.cpp
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                if (mouseX >= listboxRect.x && mouseX <= listboxRect.x + listboxRect.w &&
                    mouseY >= listboxRect.y && mouseY <= listboxRect.y + listboxRect.h) {
                    int clickedIndex = (mouseY - listboxRect.y) / itemHeight;
                    if (clickedIndex >= 0 && clickedIndex < (int)items.size()) {
                        selectedIndex = clickedIndex;
                    }
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_DOWN && selectedIndex < (int)items.size() - 1) {
                    selectedIndex++;
                } else if (e.key.keysym.sym == SDLK_UP && selectedIndex > 0) {
                    selectedIndex--;
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_RenderClear(renderer);

        // Draw listbox background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &listboxRect);
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_RenderDrawRect(renderer, &listboxRect);

        // Draw items
        for (size_t i = 0; i < items.size(); ++i) {
            SDL_Rect itemRect = {listboxRect.x, listboxRect.y + (int)i * itemHeight, listboxRect.w, itemHeight};

            // Highlight selected item
            if ((int)i == selectedIndex) {
                SDL_SetRenderDrawColor(renderer, 51, 153, 255, 255);
                SDL_RenderFillRect(renderer, &itemRect);
            }

            // Render text
            SDL_Color textColor = ((int)i == selectedIndex) ? SDL_Color{255, 255, 255, 255} : SDL_Color{0, 0, 0, 255};
            SDL_Surface* surf = TTF_RenderText_Blended(font, items[i].c_str(), textColor);
            if (surf) {
                SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                SDL_Rect destRect = {itemRect.x + 8, itemRect.y + (itemHeight - surf->h) / 2, surf->w, surf->h};
                SDL_RenderCopy(renderer, tex, NULL, &destRect);
                SDL_DestroyTexture(tex);
                SDL_FreeSurface(surf);
            }
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
