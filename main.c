#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include <math.h>
#include <SDL_ttf.h>
#include <time.h>
#include <stdbool.h>

#define FPS 60
#define TICK 1000/FPS
#define BACKGROUND "../assets/Imagenes/bg.png"
#define BACKGROUNDINGAME "../assets/Imagenes/bgingame_2.png"
#define YOUWIN "../assets/Imagenes/youwin.png"
#define YOULOSE "../assets/Imagenes/youlose.png"
#define TORRETA1 "../assets/Imagenes/archer.png"

typedef enum{
    MENU,
    JUGANDO,
    WIN,
    LOSE
}EstadoJuego;

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
    int direccion;
} Enemigo;

typedef struct{
    int vida;
    int puntos;
    int ronda;
} Jugador;

const char *frases[] = {
        "El cuidado del medio ambiente es responsabilidad de todos",
        "Reducir, reutilizar y reciclar: claves para conservar nuestro entorno",
        "La naturaleza nos brinda vida, cuidemosla con amor",
        "Cada accion cuenta en la proteccion del medio ambiente",
        "La biodiversidad es esencial para el equilibrio de nuestro planeta",
        "El cambio climatico es una realidad que debemos enfrentar juntos",
        "Fomentar la educacion ambiental es clave para un futuro sostenible",
        "La contaminacion afecta a todos, tomemos medidas para reducirla",
        "Los recursos naturales son limitados, debemos usarlos con prudencia",
        "Las energias renovables son la clave para un futuro más limpio",
        "Plantar arboles es una forma efectiva de combatir la deforestacion",
        "Cuidar los oceanos es fundamental para preservar la vida marina",
        "Los gestos cotidianos son clave para preservar nuestro entorno ambiental",
        "El consumo responsable es un paso importante hacia la sostenibilidad",
        "Apostar por la movilidad sostenible reduce la huella de carbono",
        "La conservacion del agua es crucial para la supervivencia de nuestro planeta",
        "Los ecosistemas saludables son fundamentales para nuestra existencia",
        "La contaminacion del aire afecta la salud de todos, actuemos para reducirla",
        "La proteccion de la fauna silvestre es esencial para mantener el equilibrio natural",
        "La agricultura sostenible garantiza la seguridad alimentaria a largo plazo",
        "La colaboracion global es necesaria para enfrentar los desafios ambientales",
        "Los plasticos desechables perjudican el medio ambiente",
        "La conservacion de los bosques es vital para contrarrestar el cambio climatico",
        "Las energias limpias son la clave para un futuro libre de emisiones",
        "La responsabilidad corporativa es crucial en la proteccion del entorno",
        "Reducir el derroche de alimentos es una forma de cuidar el planeta",
        "Las acciones locales tienen un impacto global en la preservacion ambiental",
        "La sensibilizacion sobre el cuidado del medio ambiente es fundamental",
        "La reforestacion es una herramienta efectiva en la lucha contra la degradacion del suelo",
        "La conservacion de la capa de ozono es esencial para proteger la vida en la Tierra",
        "La gestion adecuada de residuos minimiza el impacto en el medio ambiente",
        "La preservacion de los humedales es crucial para la biodiversidad",
        "El ecoturismo contribuye a la conservacion de areas naturales",
        "El cuidado de los polinizadores es vital para la produccion de alimentos",
        "La eficiencia energetica es clave para reducir el consumo de recursos",
        "La proteccion de los hábitats naturales es una prioridad para la conservacion",
        "La sostenibilidad debe ser parte integral de nuestras decisiones diarias",
        "La reduccion de emisiones de gases de efecto invernadero es urgente",
        "La proteccion de los arrecifes de coral es esencial para la vida marina",
        "La transicion hacia una economia circular minimiza el desperdicio",
        "La conservacion de la fauna en peligro de extincion es una responsabilidad compartida",
        "El uso responsable de la tierra preserva su fertilidad a largo plazo",
        "La educacion ambiental desde temprana edad crea conciencia sobre la importancia de la naturaleza",
        "El respeto por la biodiversidad garantiza un ecosistema equilibrado",
        "Las energias limpias ofrecen soluciones a los problemas climaticos",
        "La conservacion de los suelos evita la erosion y perdida de nutrientes",
        "La preservacion de los glaciares es crucial para el equilibrio hidrico",
        "La adopcion de habitos ecoamigables es clave para el futuro del planeta",
        "La cooperacion internacional es esencial para abordar problemas ambientales globales",
        "El amor y cuidado por la Tierra son la base de un futuro sostenible para las generaciones venideras"
};

void transformar_coordenadas(int x, int y, int windowWidth, int windowHeight, int *relativeX, int *relativeY) {
    double scaleX = (double) windowWidth / 1920;
    double scaleY = (double) windowHeight / 1080;

    *relativeX = (int) (x * scaleX);
    *relativeY = (int) (y * scaleY);
}

int generarNumeroAleatorio() {
    srand(time(NULL)); // Semilla basada en el tiempo actual para una mejor aleatoriedad
    return (rand() % 50); // Genera un número aleatorio entre 1 y 50
}

void dibujarFraseAleatoria(SDL_Renderer* renderer, TTF_Font* font, int ventana_ancho, int ventana_alto, int indiceFrase) {
    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, frases[indiceFrase], color);

    // Verifica si hay error al crear la superficie
    if (!textSurface) {
        SDL_Log("Error al renderizar la superficie del texto: %s", TTF_GetError());
        return;
    }

    // Calcula el tamaño proporcional del texto basado en la ventana
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    float scaleX = (float)ventana_ancho * 0.8 / textWidth; // Escala en el ancho
    float scaleY = (float)ventana_alto * 0.8 / textHeight; // Escala en la altura

    // Encuentra la escala mínima para mantener la proporción
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    // Aplica la escala al ancho y alto del texto
    textWidth *= scale;
    textHeight *= scale;

    // Define la posición relativa del texto en la ventana
    int x = (ventana_ancho - textWidth) / 2;
    int y = (ventana_alto - textHeight)/ 2.3;

    // Crea una textura desde la superficie del texto
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_Log("Error al crear la textura del texto: %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // Define el rectángulo que representa el área donde se mostrará el texto
    SDL_Rect destRect = { x, y, textWidth, textHeight };

    // Renderiza la textura del texto en la ventana
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    // Libera la textura y la superficie del texto
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);

    // Actualiza la ventana con el texto renderizado
    SDL_RenderPresent(renderer);
}


//Función dibujarResultado: dibuja por pantalla el mensaje de victoria o derrota
void dibujarResultado(SDL_Renderer* renderer, TTF_Font* font, int ventana_ancho, int ventana_alto, bool gano) {
    SDL_Color color;
    const char* mensaje;

    if (gano) {
        color.r = 0;
        color.g = 0;
        color.b = 255;
        color.a = 255; // Color azul (RGBA)
        mensaje = "VICTORIA!!!";
    } else {
        color.r = 255;
        color.g = 0;
        color.b = 0;
        color.a = 255; // Color rojo (RGBA)
        mensaje = "DERROTA :(";
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, mensaje, color);


    // Verifica si hay error al crear la superficie
    if (!textSurface) {
        SDL_Log("Error al renderizar la superficie del texto: %s", TTF_GetError());
        return;
    }

    // Calcula el tamaño proporcional del texto basado en la ventana
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    float scaleX = (float)ventana_ancho * 0.8 / textWidth; // Escala en el ancho
    float scaleY = (float)ventana_alto * 0.8 / textHeight; // Escala en la altura

    // Encuentra la escala mínima para mantener la proporción
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    // Aplica la escala al ancho y alto del texto
    textWidth *= scale;
    textHeight *= scale;

    // Define la posición relativa del texto en la ventana
    int x = (ventana_ancho - textWidth) / 2;
    int y = (ventana_alto - textHeight) / 2.3;

    // Crea una textura desde la superficie del texto
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_Log("Error al crear la textura del texto: %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // Define el rectángulo que representa el área donde se mostrará el texto
    SDL_Rect destRect = { x, y, textWidth, textHeight };

    // Renderiza la textura del texto en la ventana
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    // Libera la textura y la superficie del texto
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);

    // Actualiza la ventana con el texto renderizado
    SDL_RenderPresent(renderer);
}


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
void dibujarImagen(SDL_Renderer* renderer, char *nombreArchivo, int x, int y, int windowWidth, int windowHeight) {
    SDL_Surface* imageSurface = IMG_Load(nombreArchivo);
    if (!imageSurface) {
        SDL_Log("Error: IMG_Load() ha fallado: %s", IMG_GetError());
        return;
    }

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (!imageTexture) {
        SDL_Log("Error: SDL_CreateTextureFromSurface() ha fallado: %s", SDL_GetError());
        SDL_FreeSurface(imageSurface);
        return;
    }

    SDL_FreeSurface(imageSurface);

    int width, height;
    SDL_QueryTexture(imageTexture, NULL, NULL, &width, &height);
    SDL_Rect destino;

    // Transformamos las coordenadas x e y a coordenadas relativas al renderizador
    int relativeX, relativeY;
    transformar_coordenadas(x, y, windowWidth, windowHeight, &relativeX, &relativeY);

    // Asignamos las coordenadas relativas al rectángulo destino
    destino.x = relativeX;
    destino.y = relativeY;
    destino.w = width;
    destino.h = height;

    SDL_RenderCopy(renderer, imageTexture, NULL, &destino);

    SDL_DestroyTexture(imageTexture);
}

void dibujarTorreta(List* listaTorretas, SDL_Renderer* renderer, int windowWidth, int windowHeight) {
    Torreta* tempTorreta = firstList(listaTorretas);
    if (tempTorreta == NULL){
        return;
    }

    while (tempTorreta != NULL) {
        if (tempTorreta->textura != NULL) {
            dibujarImagen(renderer, TORRETA1, tempTorreta->coordenadas.Xpos, tempTorreta->coordenadas.Ypos,windowWidth, windowHeight);
        }
        tempTorreta = nextList(listaTorretas);
    }
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

int esPosicionValidaTorreta(List* listaTorretas, int x, int y, SDL_Window* window){

    Torreta* tempTorreta = firstList(listaTorretas);

    // Obtener el ancho y el alto de la ventana actual
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Calcular la relación de aspecto de la ventana respecto a la resolución original
    float aspectRatio = (float)windowWidth / 1920.0f;

    while (tempTorreta != NULL) {

        if (x >= tempTorreta->coordenadas.Xpos * aspectRatio - 100 * aspectRatio && x <= tempTorreta->coordenadas.Xpos * aspectRatio + 100 * aspectRatio &&
            y >= tempTorreta->coordenadas.Ypos * aspectRatio - 50 * aspectRatio && y <= tempTorreta->coordenadas.Ypos * aspectRatio + 50 * aspectRatio) {
            SDL_Log("Posición inválida torre: Superposición con otra torreta");
            return 0;
        }
        tempTorreta = nextList(listaTorretas);
    }

    // Ajustar las posiciones válidas según la relación de aspecto
    if (x >= 604 * aspectRatio && x <= 708 * aspectRatio && y >= 233 * aspectRatio && y <= 276 * aspectRatio) {
        SDL_Log("Posición válida 1");
        return 1;
    } else if (x >= 1240 * aspectRatio && x <= 1345 * aspectRatio && y >= 244 * aspectRatio && y <= 289 * aspectRatio) {
        SDL_Log("Posición válida 2");
        return 1;
    } else if (x >= 295 * aspectRatio && x <= 400 * aspectRatio && y >= 372 * aspectRatio && y <= 415 * aspectRatio) {
        SDL_Log("Posición válida 3");
        return 1;
    } else if (x >= 609 * aspectRatio && x <= 734 * aspectRatio && y >= 464 * aspectRatio && y <= 520 * aspectRatio) {
        SDL_Log("Posición válida 4");
        return 1;
    } else if (x >= 669 * aspectRatio && x <= 802 * aspectRatio && y >= 928 * aspectRatio && y <= 976 * aspectRatio) {
        SDL_Log("Posición válida 5");
        return 1;
    } else if (x >= 1270 * aspectRatio && x <= 1411 * aspectRatio && y >= 671 * aspectRatio && y <= 724 * aspectRatio) {
        SDL_Log("Posición válida 6");
        return 1;
    } else if (x >= 1651 * aspectRatio && x <= 1805 * aspectRatio && y >= 942 * aspectRatio && y <= 990 * aspectRatio) {
        SDL_Log("Posición válida 7");
        return 1;
    }

    // La posición no está en ningún rango permitido
    SDL_Log("Posición inválida lugar");
    return 0;
}


void colocarTorreta(List* listaTorretas, Jugador* jugador, SDL_Renderer* Renderer, int tipoTorreta, int posicionX, int posicionY,SDL_Window* window) {
    int costoTorreta;

    costoTorreta = 25;  // Costo arbitrario según el tipo

    if (jugador->puntos < costoTorreta) {
        SDL_Log("Fondos insuficientes");
        return;
    }

    // Verificar si la posición es válida
    if (!esPosicionValidaTorreta(listaTorretas, posicionX, posicionY, window)) {
        // Posición inválida
        return;
    }

    // Creación y asignación de la torreta
    Torreta *nuevaTorreta = (Torreta*)malloc(sizeof(Torreta));
    nuevaTorreta->dmg = 2;
    nuevaTorreta->coordenadas.Xpos = posicionX;
    nuevaTorreta->coordenadas.Ypos = posicionY;
    nuevaTorreta->costo = costoTorreta;

    jugador->puntos -= costoTorreta;

    // Agregar la torreta a la lista
    pushFront(listaTorretas, nuevaTorreta);

    // Verificar si las coordenadas se han guardado correctamente
    Torreta* torretaVerificacion = firstList(listaTorretas);
    if (torretaVerificacion != NULL) {
        SDL_Log("Coordenadas de la torreta en la lista - X: %d, Y: %d", torretaVerificacion->coordenadas.Xpos, torretaVerificacion->coordenadas.Ypos);
    } else {
        SDL_Log("Error: La torreta no se ha agregado correctamente a la lista.");
    }

    // Cambiar el código para dibujar la torreta según sus coordenadas reales
    //dibujarImagen(Renderer, TORRETA1, posicionX, posicionY);
}

// Esta función solo funciona para la torreta que detecta ataques en su area
void atacarEnemigos(List* listaTorretas, List* listaEnemigos, Jugador* jugador, SDL_Renderer* renderer){
    Torreta *curTorreta = firstList(listaTorretas);
    if (curTorreta != NULL) {
        if (curTorreta->textura != NULL) {
            Enemigo *curEnemigo = firstList(listaEnemigos);
            Enemigo *enemigoADisparar = NULL;

            long posX = curTorreta->coordenadas.Xpos;
            long posY = curTorreta->coordenadas.Ypos;

            long disX, disY, radio, prioridadRadio, prioridadEnemigo;

            radio = 0;
            prioridadRadio = 0;

            while (curEnemigo != NULL){

                long enX = curEnemigo->posicion.Xpos + 100; // tendría que ir sumado a algo para que se conozca más el "área" por donde camina el enemigo en ese momento
                long enY = curEnemigo->posicion.Ypos + 50;
                radio = sqrt(pow(disX, 2) + pow(disY, 2));

                if (curEnemigo->vida != 0 && radio <= curTorreta->radio && prioridadEnemigo <= enX) {
                    prioridadRadio = radio;
                    prioridadEnemigo = enX;
                    enemigoADisparar = curEnemigo;

                    // Giro
                    if (enX - posX == 0 & enY - posY > 0) {
                        curTorreta->angulo = 90;
                    } else if (enX - posX == 0 & enY - posY < 0) {
                        curTorreta->angulo = -90;
                    } else {
                        curTorreta->angulo = atan((enY - posY) / (enX - posX)) * 180 / 3.14159265;
                    }
                    if (enX - posX < 0) {
                        curTorreta->angulo += 180;
                    }

                    if (enemigoADisparar != NULL) {
                        enemigoADisparar->vida -= curTorreta->dmg;
                        // Asegurarse de que la vida no se vuelva negativa
                        enemigoADisparar->vida = fmax(enemigoADisparar->vida, 0);
                    }

                }
                curEnemigo = nextList(listaEnemigos);
                curTorreta = nextList(listaTorretas);
            }
        }
    }
}

void cargarEnemigos(List* listaEnemigos, int ronda){
    int cantEnemigosSpawnear = 5 * ronda + 1;
    int i = 0;
    while (i < cantEnemigosSpawnear){
        Enemigo* nuevoEnemigo = (Enemigo*)malloc(sizeof(Enemigo));
        if (ronda == 1){
            nuevoEnemigo->tipo = "a";
            nuevoEnemigo->velocidad = 3;
            nuevoEnemigo->vida = 3;
            nuevoEnemigo->daño = 3;
            nuevoEnemigo->premio = 100;
            SDL_Log("Enemigo %d: %s", i, nuevoEnemigo->tipo);
        } else if (ronda == 2){
            nuevoEnemigo->tipo = "b";
            nuevoEnemigo->velocidad = 5;
            nuevoEnemigo->vida = 5;
            nuevoEnemigo->daño = 5;
            nuevoEnemigo->premio = 200;
            SDL_Log("Enemigo %d: %s", i, nuevoEnemigo->tipo);
        } else if (ronda == 3){
            nuevoEnemigo->tipo = "c";
            nuevoEnemigo->velocidad = 1;
            nuevoEnemigo->vida = 20;
            nuevoEnemigo->daño = 20;
            nuevoEnemigo->premio = 500;
            SDL_Log("Enemigo %d: %s", i, nuevoEnemigo->tipo);
        } else if (ronda >= 4){
            if (i <= 5){
                nuevoEnemigo->tipo = "a";
                nuevoEnemigo->velocidad = 3;
                nuevoEnemigo->vida = 3;
                nuevoEnemigo->daño = 3;
                nuevoEnemigo->premio = 100;
                SDL_Log("Enemigo %d: %s", i, nuevoEnemigo->tipo);
            } else if (i <= 10 && i > 5){
                nuevoEnemigo->tipo = "b";
                nuevoEnemigo->velocidad = 5;
                nuevoEnemigo->vida = 5;
                nuevoEnemigo->daño = 5;
                nuevoEnemigo->premio = 200;
                SDL_Log("Enemigo %d: %s", i, nuevoEnemigo->tipo);
            } else {
                nuevoEnemigo->tipo = "c";
                nuevoEnemigo->velocidad = 1;
                nuevoEnemigo->vida = 20;
                nuevoEnemigo->daño = 20;
                nuevoEnemigo->premio = 500;
                SDL_Log("Enemigo %d: %s", i, nuevoEnemigo->tipo);
            }
        }
        pushFront(listaEnemigos, nuevoEnemigo);
        i++;
    }
}

void moverEnemigo(Enemigo* arregloEnemigos, SDL_Window* window) {
    int i = 0;
    int anchoPantalla, altoPantalla;
    SDL_GetWindowSize(window, &anchoPantalla, &altoPantalla);
    float relacionAncho = (float)anchoPantalla / 1920.0f;
    float relacionAlto = (float)altoPantalla / 1080.0f;
    while (arregloEnemigos[i].tipo != NULL) {
        if (arregloEnemigos[i].tipo == "a" || arregloEnemigos[i].tipo == "b" || arregloEnemigos[i].tipo == "c") {
            // Movimiento según el camino especificado
            switch(arregloEnemigos[i].direccion){
                case 1:
                    arregloEnemigos[i].posicion.Ypos += arregloEnemigos[i].velocidad * relacionAlto;
                    break;
                case 2:
                    arregloEnemigos[i].posicion.Xpos -= arregloEnemigos[i].velocidad * relacionAncho;
                    break;
                case 3:
                    arregloEnemigos[i].posicion.Ypos -= arregloEnemigos[i].velocidad * relacionAlto;
                    break;
                case 4:
                    arregloEnemigos[i].posicion.Xpos -= arregloEnemigos[i].velocidad * relacionAncho;
                    break;
            }
        }
        if (arregloEnemigos[i].posicion.Ypos >= 130 * relacionAlto && arregloEnemigos[i].direccion == 1){
            arregloEnemigos[i].direccion = 4;
        } else if (arregloEnemigos[i].posicion.Xpos <= 505 * relacionAncho && arregloEnemigos[i].direccion == 2){
            arregloEnemigos[i].direccion = 3;
        } else if (arregloEnemigos[i].posicion.Ypos <= 505 * relacionAlto && arregloEnemigos[i].direccion == 4){
            arregloEnemigos[i].direccion = 1;
        } else if ((arregloEnemigos[i].posicion.Xpos >= 500 * relacionAncho && arregloEnemigos[i].posicion.Xpos <= 510 * relacionAncho) && (arregloEnemigos[i].posicion.Ypos >= 500 * relacionAlto && arregloEnemigos[i].posicion.Ypos <= 510 * relacionAlto) && (arregloEnemigos[i].direccion == 3 || arregloEnemigos[i].direccion == 1)){
            arregloEnemigos[i].direccion = 2;
        }
        i++;
    }
}


void generarEnemigo(List* listaEnemigos, Enemigo* arregloEnemigos){
    Enemigo* tempEnemigo = firstList(listaEnemigos);
    Uint32 tiempoTrancurrido = 0;
    int i=0;
    while (tempEnemigo != NULL){
        tiempoTrancurrido += TICK;
        if (tiempoTrancurrido >= 1000000000){
            arregloEnemigos[i] = *tempEnemigo;
            if (i%2 == 0){
                arregloEnemigos[i].posicion.Xpos = 1280;
                arregloEnemigos[i].posicion.Ypos = 0;
                arregloEnemigos[i].direccion = 1;
            } else {
                arregloEnemigos[i].posicion.Xpos = 1920;
                arregloEnemigos[i].posicion.Ypos = 815;
                arregloEnemigos[i].direccion = 2;
            }
            tempEnemigo = nextList(listaEnemigos);
            popFront(listaEnemigos);
            SDL_Log("Enemigo %d: %s elpepekbron", i, arregloEnemigos[i].tipo);
            i++;
            tiempoTrancurrido = 0;
        }
    }

}

int WinMain(int argc, char* argv[]) {

    EstadoJuego estadoActual = MENU;
    int numeroAleatorio = generarNumeroAleatorio();

    List* listaTorretas = createList();
    List* listaEnemigos = createList();
    Enemigo* enemigos = malloc(sizeof(Enemigo) * 100);
    Torreta* tempTorreta = firstList(listaTorretas);

    Jugador jugador;  // Creas una instancia de la estructura Jugador
    jugador.vida = 100;  // Puedes ajustar este valor según tus necesidades iniciales
    jugador.puntos = 1000;
    jugador.ronda = 1;

    if (TTF_Init() != 0) {
        SDL_Log("Error al inicializar SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Font* fuente = TTF_OpenFont("../assets/Fuentes/Minecraft.ttf", 50);
    if (!fuente) {
        SDL_Log("Error al cargar la fuente: %s", TTF_GetError());
    }

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

    const int maxWidth = 1980;
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
    Uint32 frameStart;
    int frameTime;
    Uint32 tiempoTranscurrido = 0;

    while (running) {
        frameTime = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            //switch de procesado de eventos
            switch (estadoActual) {
                case MENU:
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
                                    numeroAleatorio = generarNumeroAleatorio();
                                    estadoActual = JUGANDO;

                                }
                                buttonY = windowHeight * (0.6 - 0.05);
                                if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth && mouseY >= buttonY && mouseY <= buttonY + buttonHeight) {
                                    numeroAleatorio = generarNumeroAleatorio();
                                    // Si el usuario hace clic en "Configuraciones".
                                    SDL_Log("Configuraciones");
                                    if (!inSettings) {
                                        int resultado = mostrarConfiguraciones(window, renderer);
                                        if (resultado == 1) {
                                            estadoActual = MENU;
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
                    break;
                case JUGANDO:
                    switch (event.type) {
                        case SDL_MOUSEBUTTONDOWN:
                            if (event.button.button == SDL_BUTTON_LEFT){
                                int mouseX = event.button.x;
                                int mouseY = event.button.y;
                                SDL_Log("X: %d, Y: %d", mouseX, mouseY);

                                tiempoTranscurrido += TICK;

                                if (mouseX >= 0 && mouseX <= windowWidth && mouseY >= 0 && mouseY <= windowHeight){

                                    int juegoMouseX = mouseX;
                                    int juegoMouseY = mouseY;

                                    if (esPosicionValidaTorreta(listaTorretas, juegoMouseX, juegoMouseY, window)){
                                        colocarTorreta(listaTorretas, &jugador, renderer, 1, juegoMouseX, juegoMouseY,window);

                                        Torreta* torretaVerificada = firstList(listaTorretas);
                                        while (torretaVerificada != NULL) {
                                            SDL_Log("X: %d, Y: %d", torretaVerificada->coordenadas.Xpos, torretaVerificada->coordenadas.Ypos);
                                            if (torretaVerificada->coordenadas.Xpos == juegoMouseX && torretaVerificada->coordenadas.Ypos == juegoMouseY) {
                                                SDL_Log("Torreta colocada en (%d, %d)", juegoMouseX, juegoMouseY);
                                                break;
                                            }
                                            torretaVerificada = nextList(listaTorretas);
                                        }

                                        if (torretaVerificada == NULL) {
                                            SDL_Log("Error al colocar la torreta en (%d, %d)", juegoMouseX, juegoMouseY);
                                        }
                                    }

                                    if (tiempoTranscurrido >= 1000){
                                        atacarEnemigos(listaTorretas, listaEnemigos, &jugador, renderer);
                                        tiempoTranscurrido = 0;
                                    }
                                }
                            }
                            break;

                        case SDL_QUIT:
                            running = 0;
                            break;
                        case SDL_KEYDOWN:
                            if (event.key.keysym.sym == SDLK_ESCAPE) {
                                estadoActual = MENU;
                            }
                            if (event.key.keysym.sym == SDLK_r) {
                                cargarEnemigos(listaEnemigos, jugador.ronda);
                                jugador.ronda++;
                            }
                            break;
                    }
                    break;
                case WIN:
                    switch (event.type) {
                        case SDL_QUIT:
                            running = 0;
                            break;
                        case SDL_KEYDOWN:
                            if (event.key.keysym.sym == SDLK_ESCAPE) {
                                estadoActual = MENU;
                            }
                            break;
                    }
                    break;
                case LOSE:
                    switch (event.type) {
                        case SDL_QUIT:
                            running = 0;
                            break;
                        case SDL_KEYDOWN:
                            if (event.key.keysym.sym == SDLK_ESCAPE) {
                                estadoActual = MENU;
                            }
                            break;
                    }
                    break;
            }

        }
        //switch para actualizar los eventos del juego
        switch (estadoActual) {
            case MENU:
                break;
            case JUGANDO:
                generarEnemigo(listaEnemigos, enemigos);
                break;
            case WIN:
                break;
            case LOSE:
                break;
        }
        //switch para renderizar los eventos del juego
        switch (estadoActual) {
            case MENU:
                dibujarFondo(BACKGROUND, windowWidth,windowHeight,window,renderer);
                dibujarFraseAleatoria(renderer, fuente, windowWidth, windowHeight,numeroAleatorio);
                break;
            case JUGANDO:
                dibujarFondo(BACKGROUNDINGAME, windowWidth,windowHeight,window,renderer);
                dibujarTorreta(listaTorretas,renderer,windowWidth,windowHeight);
                break;
            case WIN:
                dibujarFondo(YOUWIN, windowWidth,windowHeight,window,renderer);
                dibujarResultado(renderer, fuente, windowWidth, windowHeight, true);
                break;
            case LOSE:
                dibujarFondo(YOULOSE, windowWidth,windowHeight,window,renderer);
                dibujarResultado(renderer, fuente, windowWidth, windowHeight, false);
                break;
        }
        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;
        if (TICK > frameTime) {
            SDL_Delay(TICK - frameTime);
        }
    }
    while (tempTorreta != NULL){
        free(tempTorreta);
        tempTorreta = nextList(listaTorretas);
    }
    free(enemigos);
    freeList(listaTorretas);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}