#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "list.h"

typedef struct {
    int Xpos;
    int Ypos;
} Punto;

typedef struct{
    Punto inicio;
    Punto final;
    Punto actual;
    char* tipo;
} Proyectil ;

typedef struct{
    SDL_Texture* textura;
    char* tipo;
    int dmg;
    Punto coordenadas;
    int nivel;
    int costo;
    int angulo;
    int radio;
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

//Esta funcion como dice su nombre dibuja el fondo ya sea del menu o del juego
void dibujarFondo(char*nombre,int x,int y,SDL_Window* window,SDL_Renderer* renderer) {
    SDL_Surface* bgSurface = IMG_Load(nombre);
    if (!bgSurface) {
        SDL_Log("Error: IMG_Load() ha fallado: %s", IMG_GetError());
        return;
    }
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    if (!bgTexture) {
        SDL_Log("Error: SDL_CreateTextureFromSurface() ha fallado: %s", SDL_GetError());
        return;
    }
    SDL_FreeSurface(bgSurface);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(bgTexture);
}

//Esta funcion dibuja cualquier imagen solo necesitas el nombre del archivo, y una posicion
//Cuando hagan el bucle del juego podre hacer que funcione bien
void dibujarImagen(SDL_Renderer* renderer, char *nombreArchivo, int x, int y) {
    SDL_Surface* imageSurface = IMG_Load(nombreArchivo);
    if (!imageSurface) {
        SDL_Log("Error: IMG_Load() ha fallado: %s", IMG_GetError());
        return;
    }
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (!imageTexture) {
        SDL_Log("Error: SDL_CreateTextureFromSurface() ha fallado: %s", SDL_GetError());
        return;
    }
    SDL_FreeSurface(imageSurface);
    int width, height;
    SDL_QueryTexture(imageTexture, NULL, NULL, &width, &height);
    SDL_Rect destino;
    destino.x = x;
    destino.y = y;
    destino.w = width;
    destino.h = height;
    SDL_RenderCopy(renderer, imageTexture, NULL, &destino);
    SDL_DestroyTexture(imageTexture);
}

int mostrarConfiguraciones(SDL_Window* window, SDL_Renderer* renderer) {
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

        SDL_RenderPresent(renderer);


    }
    return 0;
}

int esPosicionValidaTorreta(List* listaTorretas, int x, int y) {
    Torreta* curTorreta = firstList(listaTorretas);

    while (curTorreta != NULL) {
        if (curTorreta->coordenadas.Xpos == x && curTorreta->coordenadas.Ypos == y){
            return 1; // La posición está ocupada.
        }
        curTorreta = nextList(listaTorretas);
    }
    // La posición no está ocupada.
    return 0;
}

// Asumí que se toman los datos del tipo y las posiciones de otro lado xd
void colocarTorreta(List* listaTorretas, Jugador* jugador, SDL_Renderer* Renderer, int tipoTorreta, int posicionX, int posicionY){

    int costoTorreta;
    // Voy a poner costos arbitrarios
    switch (tipoTorreta) {
        case 1:
            costoTorreta = 25;
            break;
        case 2:
            costoTorreta = 50;
            break;
        case 3:
            costoTorreta = 75;
            break;
        default:
            // si el tipo de torreta no coincide
            return;
    }

    if (jugador->puntos < costoTorreta) {
        // jugador no tiene suficiente dinero
        return;
    }

    // Creación de la torreta
    Torreta* nuevaTorreta = malloc(sizeof(Torreta));

    // Seleccionar tipo de torreta
    switch(tipoTorreta){
        case 1:
            nuevaTorreta->tipo = "Torreta tipo 1";
            //aquí irían las stats de la torreta que aún no definimos
            //además de como se le asigna una textura
            break;
        case 2:
            nuevaTorreta->tipo = "Torreta tipo 2";
            break;
        case 3:
            nuevaTorreta->tipo = "Torreta tipo 3";
            break;
        default:
            //si el tipo de torreta no coincide
            free(nuevaTorreta);
            return;
    }

    nuevaTorreta->coordenadas.Xpos = posicionX;
    nuevaTorreta->coordenadas.Ypos = posicionY;

    nuevaTorreta->costo = costoTorreta;

    jugador->puntos -= costoTorreta;

    pushBack(listaTorretas, nuevaTorreta);

    SDL_Rect Rect; // esto crea un rectangulo para que se pueda apretar la torreta en un area determinada y no un pixel
    SDL_RendererFlip Flip = SDL_FLIP_NONE; // Esto creo que hace que la torreta gire
    Torreta* curTorreta = firstList(listaTorretas);

    while (curTorreta != NULL){
        if(curTorreta->textura != NULL){
            curTorreta->angulo = 180;
            Rect.x = posicionX;
            Rect.y = posicionY;
            //Rect.w = ancho de la torreta
            //Rect.h = alto de la torreta

            SDL_RenderCopyEx(Renderer, curTorreta->textura, NULL, &Rect, curTorreta->angulo, NULL, Flip);
        }
        curTorreta = nextList(listaTorretas);
    }
}

/*Torreta* clickEnTorreta(List* listaTorretas, float x, float y){
    Torreta* curTorreta = (firstList(listaTorretas);

    while (curTorreta != NULL) {
        if (curTorreta->coordenadas.Xpos <= x && x <= curTorreta->coordenadas.Xpos &&
            curTorreta->coordenadas.Ypos <= y && y <= curTorreta->coordenadas.Ypos){
            return curTorreta;
        }

        curTorreta = nextList(listaTorretas);
    }

    return NULL;
}*/

/*void eliminarTorreta(List* listaTorretas, Jugador* jugador, float x, float y){
    Torreta* curTorreta = firstList(listaTorretas);

    while (curTorreta != NULL){
        if (curTorreta->coordenadas.Xpos == x && curTorreta->coordenadas.Ypos == y){
            jugador->puntos += curTorreta->costo;
            popCurrent(listaTorretas);
            free(curTorreta);
            return;
        }
        curTorreta = (Torreta*)nextList(listaTorretas);
    }
}*/

// Esta función solo funciona para la torreta que detecta ataques en su area
void atacarEnemigos(List* listaTorretas, List* listaEnemigos, Jugador* jugador, SDL_Renderer* renderer){

    Torreta* curTorreta = firstList(listaTorretas);

    if (curTorreta != NULL){
        if (curTorreta != NULL){
            Enemigo* curEnemigo = firstList(listaEnemigos);
            Enemigo* enemigoADisparar = NULL;

            double posX = curTorreta->coordenadas.Xpos;
            double posY = curTorreta->coordenadas.Ypos;

            double disX, disY, radio, prioridadRadio, prioridadEnemigo;

            radio = 0;
            prioridadRadio = 0;

            while(curEnemigo != NULL){

                double enX = curEnemigo->posicion.Xpos; // tendría que ir sumado a algo para que se conozca más el "área" por donde camina el enemigo en ese momento
                double enY = curEnemigo->posicion.Ypos;


                radio = sqrt(pow(disX, 2) + pow(disY, 2));

                if(curEnemigo->vida != 0 && radio <= curTorreta->radio && prioridadEnemigo <= enX){
                    prioridadRadio = radio;
                    prioridadEnemigo = enX;
                    enemigoADisparar = curEnemigo;

                    // Giro
                    if(enX - posX == 0 & enY - posY > 0){
                        curTorreta->angulo = 90;
                    }
                    else if(enX - posX == 0 & enY - posY < 0){
                        curTorreta->angulo = -90;
                    }
                    else{
                        curTorreta->angulo = atan((enY- posY) / (enX - posX)) * 180 / 3.14159265;
                    }
                    if(enX -posX < 0){
                        curTorreta->angulo += 180;
                    }

                    // Aquí debería ir updateProyectil

                }
                curEnemigo = nextList(listaEnemigos);
            }
        }
        curTorreta = nextList(listaTorretas);

    }
}

int WinMain(int argc, char* argv[]) {

    List* listaTorretas = createList();
    List* listaEnemigos = createList();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error al inicializar SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Surface* bgSurface = IMG_Load("../assets/Imagenes/bgingame.png");
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
    int X,Y;

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
                    // si apretas una posición, te dice lo que apretaste, en consola dice
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;

                        int buttonWidth = windowWidth * 0.11;
                        int buttonHeight = windowHeight * 0.08;
                        int buttonX = windowWidth * (0.55 - 0.08);
                        int buttonY = windowHeight * (0.5 - 0.05);
                        SDL_GetMouseState(&X,&Y);
                        SDL_Log("X: %d Y: %d",X,Y);

                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {

                            SDL_Log("Jugar");
                            char*nombre={"../assets/Imagenes/ll.png"};
                            dibujarFondo(nombre, windowWidth,windowHeight,window,renderer);
                        }
                        buttonY = windowHeight * (0.6 - 0.05);
                        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                            // Si el usuario hace clic en "Configuraciones".
                            SDL_Log("Configuraciones");
                            if (!inSettings) {
                                int resultado = mostrarConfiguraciones(window, renderer);
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