#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>

void drawText(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, const char* text, float x, float y, float w, float h) {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int posX = windowWidth * x;
    int posY = windowHeight * y;
    int width = windowWidth * w;
    int height = windowHeight * h;

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, (SDL_Color){0, 0, 0, 255});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {posX, posY, width, height};

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

int showSettings(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    int running = 1;
    SDL_Event event;

    SDL_Surface* bgSurface = IMG_Load("../assets/Imagenes/bg.png");
    if (!bgSurface) {
        SDL_Log("Error: IMG_Load() ha fallado: %s", IMG_GetError());
        return 0;
    }

    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);

    SDL_FreeSurface(bgSurface);


    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        return 1;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;

                        int buttonWidth = windowWidth * 0.11;
                        int buttonHeight = windowHeight * 0.08;
                        int buttonX = windowWidth * (0.5 - 0.1);
                        int buttonY = windowHeight * (0.75 - 0.05);

                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            Uint32 flag = SDL_GetWindowFlags(window);
                            if (flag & SDL_WINDOW_FULLSCREEN) {
                                SDL_SetWindowFullscreen(window, 0);
                                SDL_Log("Pantalla completa  ✘");
                            } else {
                                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                                SDL_Log("Pantalla completa  ✔");
                            }
                        }

                        buttonY = windowHeight * (0.82 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            SDL_Log("Volumen musica");
                        }

                        buttonY = windowHeight * (0.89 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {

                            SDL_Log("Volumen musica");
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

        Uint32 flag = SDL_GetWindowFlags(window);
        if (flag & SDL_WINDOW_FULLSCREEN){
            drawText(window, renderer, font, "Pantalla completa : Si", 0.5 - 0.1, 0.75 - 0.05, 0.17, 0.08);
        }else{
            drawText(window, renderer, font, "Pantalla completa : No", 0.5 - 0.1, 0.75 - 0.05, 0.17, 0.08);
        }
        drawText(window, renderer, font, "Volumen musica", 0.5 - 0.1, 0.82 - 0.05, 0.17, 0.08);
        drawText(window, renderer, font, "Volumen musica", 0.5 - 0.1, 0.89 - 0.05, 0.17, 0.08);

        SDL_RenderPresent(renderer);
    }
    return 0;
}

int WinMain(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error: SDL_Init() ha fallado: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        SDL_Log("Error: TTF_Init() ha fallado: %s", TTF_GetError());
        return 1;
    }

    SDL_Surface* bgSurface = IMG_Load("../assets/Imagenes/bg.png");
    if (!bgSurface) {
        SDL_Log("Error: IMG_Load() ha fallado: %s", IMG_GetError());
        return 1;
    }

    // Cargar el logo
    SDL_Surface* logoSurface = IMG_Load("../assets/Imagenes/cartel.png");
    if (!logoSurface) {
        SDL_Log("Error: IMG_Load() ha fallado: %s", IMG_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Eco Defender", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920 , 1080, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_Texture* logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    TTF_Font* font = TTF_OpenFont("../assets/Fuentes/lato/Lato-Black.ttf", 24);

    SDL_FreeSurface(bgSurface);
    SDL_FreeSurface(logoSurface);

    SDL_Event event;
    int running = 1;
    int inSettings=0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = 0;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;

                        int buttonWidth = windowWidth * 0.11;
                        int buttonHeight = windowHeight * 0.08;
                        int buttonX = windowWidth * (0.5 - 0.1);
                        int buttonY = windowHeight * (0.75 - 0.05);

                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            // Si el usuario hace clic en "Jugar".
                            SDL_Log("Jugar");
                            SDL_SetWindowFullscreen(window,1);
                        }
                        buttonY = windowHeight * (0.82 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            // Si el usuario hace clic en "Configuraciones".
                            SDL_Log("Configuraciones");
                            if (!inSettings) {
                                int resultado = showSettings(window, renderer, font);
                                if (resultado == 1) {
                                    inSettings = 0;
                                }else{
                                    inSettings=1;
                                }
                            }
                        }

                        buttonY = windowHeight * (0.89 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            // Si el usuario hace clic en "Puntuaciones".
                            SDL_Log("Puntuaciones");
                        }

                        buttonY = windowHeight * (0.95 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            // Si el usuario hace clic en "Salir".
                            running = 0;
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

        int logoWidth = windowWidth / 4;
        int logoHeight = windowHeight / 2.5;
        int logoX = windowWidth / 2 - logoWidth / 2;
        int logoY = windowHeight / 2 - logoHeight / 2;
        SDL_Rect logoRect = {logoX,logoY, logoWidth, logoHeight};
        SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);

        drawText(window, renderer, font, "Jugar", 0.55 - 0.1, 0.75 - 0.05, 0.06, 0.08);
        drawText(window, renderer, font, "Configuraciones", 0.5 - 0.1, 0.82 - 0.05, 0.17, 0.08);
        drawText(window, renderer, font, "Puntuaciones", 0.5 - 0.1, 0.89 - 0.05, 0.17, 0.08);
        drawText(window, renderer, font, "Salir", 0.55 - 0.1, 0.95 - 0.05, 0.06, 0.08);


        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}