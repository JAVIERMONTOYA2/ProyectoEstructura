#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    float Xpos;
    float Ypos;
} Punto;

typedef struct{
    Punto inicio;
    Punto final;
    Punto actual;
    char* tipo;
} Proyectil ;

typedef struct{
    char* tipo;
    int dmg;
    Punto coordenadas;
    int nivel;
    int costo;
} Torreta;

typedef struct{
    char* tipo;
    float velocidad;
    Punto posicion;
    int vida;
    int daño;
    int premio;
} Enemigo;

typedef struct{
    int vida;
    int puntos;
    int ronda;
} Jugador;

int showSettings(SDL_Window* window, SDL_Renderer* renderer) {
    int running = 1;
    SDL_Event event;

    SDL_Surface* bgSurface = IMG_Load("../assets/Imagenes/clean.png");
    if (!bgSurface) {
        SDL_Log("Error: IMG_Load() ha fallado: %s", IMG_GetError());
        return 0;
    }

    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);

    SDL_FreeSurface(bgSurface);

    SDL_Texture* fullscreenTexture = IMG_LoadTexture(renderer, "../assets/Imagenes/fullscreen.png");
    if (!fullscreenTexture) {
        SDL_Log("Error: IMG_LoadTexture() ha fallado: %s", IMG_GetError());
        return 0;
    }

    /*SDL_Texture* musicVolumeTexture = IMG_LoadTexture(renderer, "../assets/Imagenes/music_volume.png");
    if (!musicVolumeTexture) {
        SDL_Log("Error: IMG_LoadTexture() ha fallado: %s", IMG_GetError());
        return 0;
    }

    SDL_Texture* soundVolumeTexture = IMG_LoadTexture(renderer, "../assets/Imagenes/sound_volume.png");
    if (!soundVolumeTexture) {
        SDL_Log("Error: IMG_LoadTexture() ha fallado: %s", IMG_GetError());
        return 0;
    }*/

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    int buttonWidth = windowWidth * 0.17;
    int buttonHeight = windowHeight * 0.08;
    int originalButtonX = windowWidth * (0.5 - 0.08);
    int originalButtonY = windowHeight * (0.5 - 0.05);

    int buttonX = originalButtonX;
    int buttonY = originalButtonY;

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

                        buttonY += windowHeight * (0.6 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            SDL_Log("Volumen musica");
                        }

                        buttonY += windowHeight * (0.71 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            SDL_Log("Volumen sonido");
                        }

                        buttonX = originalButtonX;
                        buttonY = originalButtonY;

                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

        Uint32 flag = SDL_GetWindowFlags(window);
        SDL_Rect srcrect;
        SDL_Rect dstrect;

        srcrect.w = 276;
        srcrect.h = 74;
        srcrect.x = 0;
        srcrect.y = 0;
        if (flag & SDL_WINDOW_FULLSCREEN) {
            srcrect.x = 0;
        } else {
            srcrect.x = 276;
        }
        dstrect.x = originalButtonX;
        dstrect.y = originalButtonY;
        dstrect.w = buttonWidth;
        dstrect.h = buttonHeight;
        SDL_RenderCopyEx(renderer, fullscreenTexture, &srcrect, &dstrect, 0, NULL, SDL_FLIP_NONE);

        /*srcrect.w = 128;
        srcrect.h = 64;
        srcrect.x = 0;
        srcrect.y = 0;

        dstrect.x = buttonX;
        dstrect.y = buttonY + buttonHeight;
        dstrect.w = buttonWidth;
        dstrect.h = buttonHeight;
        SDL_RenderCopyEx(renderer, musicVolumeTexture, &srcrect, &dstrect, 0, NULL, SDL_FLIP_NONE);

        srcrect.w = 128;
        srcrect.h = 64;
        srcrect.x = 0;
        srcrect.y = 0;

        dstrect.x = buttonX;
        dstrect.y = buttonY + buttonHeight * 2;
        dstrect.w = buttonWidth;
        dstrect.h = buttonHeight;
        SDL_RenderCopyEx(renderer, soundVolumeTexture, &srcrect, &dstrect, 0, NULL, SDL_FLIP_NONE);*/

        SDL_RenderPresent(renderer);


    }
    return 0;
}


int WinMain(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error al inicializar SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Surface* bgSurface = IMG_Load("../assets/Imagenes/bg.png");
    if (!bgSurface) {
        SDL_Log("Error: IMG_Load() ha fallado: %s", IMG_GetError());
        return 1;
    }
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        SDL_Log("Error al obtener la resolución de la pantalla: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    const int maxWidth = 1920;
    const int maxHeight = 1080;

    int windowWidth = dm.w;
    int windowHeight = dm.h;

    if (windowWidth > maxWidth || windowHeight > maxHeight) {
        windowWidth = maxWidth;
        windowHeight = maxHeight;
    }

    SDL_Window* window = SDL_CreateWindow("Eco Defender", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Log("Error al crear la ventana: %s", SDL_GetError());
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);


    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    SDL_FreeSurface(bgSurface);


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
                        int buttonX = windowWidth * (0.55 - 0.08);
                        int buttonY = windowHeight * (0.5 - 0.05);

                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {

                            SDL_Log("Jugar");

                        }
                        buttonY = windowHeight * (0.6 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            // Si el usuario hace clic en "Configuraciones".
                            SDL_Log("Configuraciones");
                            if (!inSettings) {
                                int resultado = showSettings(window, renderer);
                                if (resultado == 1) {
                                    inSettings = 0;
                                }else{
                                    inSettings=1;
                                }
                            }
                        }

                        buttonY = windowHeight * (0.71 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            SDL_Log("Puntuaciones");
                        }

                        buttonY = windowHeight * (0.84 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            running = 0;
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);


        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}