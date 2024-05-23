// Librerias implementadas
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

//Definiciones referentes a la infraestructura del juego
#define ANCHO 800
#define ALTO 800
#define FPS 60
#define TPS 7 
#define TAM_CELDA 38
#define ANCHO_CELDA 19
#define ALTO_CELDA 19

// Constantes empleadas
const char TITULO[] = "Snaky";
const Color color_fondo = {255, 235, 39, 255};
const Color color_tablero_1 = {40, 121, 0, 255};
const Color color_tablero_2 = {51, 153, 0, 255};
const Color color_serpiente = {255, 0, 204, 255};
const Color color_oscuro = {0, 0, 0, 100};

// Texturas de las imagenes cargadas en el juego
Texture2D manzana_textura;
Texture2D boton_inicio_textura;

// Estructura para representar un nodo de la serpiente
typedef struct nodo 
{
    int x, y; 
    struct nodo *siguiente;
} nodo;

// Estructura para representar el juego
typedef struct juego 
{
    unsigned frames_transcurridos;
    bool juego_terminado;
    bool empezado;
    nodo *serpiente;
    int manzana_x, manzana_y, dx, dy, tamano;
    int puntaje;
} juego;

// Declaracion de funciones
void colocar_manzana(juego *juego);
void reiniciar(juego *juego);
void dibujar(juego *juego);
void incrementar_puntaje(juego *juego);
void resetear_puntaje(juego *juego);
void dibujar_puntaje(juego *juego);
void actualizar(juego *juego);

int main() 
{
    juego juego;

    srand((unsigned)time(NULL)); // Inicializacion de la semilla para la generacion de numeros aleatorios
    assert(!GetWindowHandle()); // Asegura que no haya ninguna ventana activa antes de iniciar
    SetTargetFPS(60); // Establece el numero de frames por segundo objetivo

    // Inicializacion de la ventana y carga de recursos
    InitWindow(ANCHO, ALTO, TITULO);
    Image icono = LoadImage("resources/SG_LogoJuego.png");
    SetWindowIcon(icono);
    Image manzanaImagen = LoadImage("resources/SG_Manzana.png");
    manzana_textura = LoadTextureFromImage(manzanaImagen);
    UnloadImage(manzanaImagen);
    Image botonInicio = LoadImage("resources/SG_BotonInicio.png");
    boton_inicio_textura = LoadTextureFromImage(botonInicio);
    UnloadImage(botonInicio);

    reiniciar(&juego); // Inicializacion del juego

    while (true) {
        if (juego.juego_terminado) {
            sleep(1); // Pausa de 1 segundo antes de reiniciar el juego
            reiniciar(&juego); // Reinicia el juego
            continue;
        }

        actualizar(&juego); // Actualiza el estado del juego
        dibujar(&juego); // Dibuja la pantalla
        dibujar_puntaje(&juego); // Dibuja el puntaje

        if (WindowShouldClose()) {
            break; // Sale del bucle si se cierra la ventana
        }
    }

    assert(GetWindowHandle()); // Asegura que la ventana este cerrada al salir del bucle
    CloseWindow(); // Cierra la ventana al salir del juego

    UnloadTexture(manzana_textura); // Libera la textura de la manzana
    UnloadTexture(boton_inicio_textura); // Libera la textura del boton de inicio
    return 0;
}

// Funcion para colocar la manzana en una posicion aleatoria en el tablero
void colocar_manzana(juego *juego) 
{
    while (1) {
        int rx = rand() % ANCHO_CELDA;
        int ry = rand() % ALTO_CELDA;

        nodo *actual = juego->serpiente;
        while (actual) {
            if (actual->x == rx && actual->y == ry) 
            {
                break;
            }
            actual = actual->siguiente;
        }
        if (!actual) 
        {
            juego->manzana_x = rx;
            juego->manzana_y = ry;
            break;
        }
    }
}

// Funcion para reiniciar el juego
void reiniciar(juego *juego) 
{
    // Reinicia todas las variables del juego a sus valores iniciales
    juego->frames_transcurridos = 0;
    juego->juego_terminado = false;
    juego->tamano = 0;
    juego->dx = 1;
    juego->dy = 0;
    juego->serpiente = NULL;
    juego->manzana_x = 0;
    juego->manzana_y = 0;
    juego->empezado = false;
    juego->puntaje = 0;
    
    // Crea la serpiente inicial
    juego->serpiente = (nodo *)malloc(sizeof(nodo));
    juego->serpiente->x = 7;
    juego->serpiente->y = 9;
    juego->serpiente->siguiente = (nodo *)malloc(sizeof(nodo));
    juego->serpiente->siguiente->x = 6;
    juego->serpiente->siguiente->y = 9;
    juego->serpiente->siguiente->siguiente = (nodo *)malloc(sizeof(nodo));
    juego->serpiente->siguiente->siguiente->x = 5;
    juego->serpiente->siguiente->siguiente->y = 9;
    juego->serpiente->siguiente->siguiente->siguiente = NULL;
    
    colocar_manzana(juego); // Coloca la manzana en una posicion aleatoria
}

// Funcion para dibujar la pantalla del juego
void dibujar(juego *juego) 
{
    BeginDrawing();
    ClearBackground(color_fondo);

    // Dibuja el tablero
    for (int i = 0; i < ALTO_CELDA; i++) 
    {
        for (int j = 0; j < ANCHO_CELDA; j++) 
        {
            DrawRectangle((j + 1) * TAM_CELDA, (i + 1) * TAM_CELDA, TAM_CELDA, TAM_CELDA,
                          i * ANCHO_CELDA + j & 1 ? color_tablero_2 : color_tablero_1);
        }
    }

    // Dibuja la serpiente
    {
        nodo *actual = juego->serpiente;
        while (actual) 
        {
            DrawRectangle((actual->x + 1) * TAM_CELDA, (actual->y + 1) * TAM_CELDA, TAM_CELDA,
                          TAM_CELDA, color_serpiente);
            actual = actual->siguiente;
        }
    }

    // Dibuja la manzana
    DrawTextureEx(manzana_textura, (Vector2){ (juego->manzana_x + 1) * TAM_CELDA, (juego->manzana_y + 1) * TAM_CELDA }, 0.0f, 1.0f, WHITE);

    // Dibuja el boton de inicio si el juego no ha comenzado
    if (!juego->empezado) 
    {
        float posX = (GetScreenWidth() - boton_inicio_textura.width) / 2;
        float posY = (GetScreenHeight() - boton_inicio_textura.height) / 2;
        DrawTexture(boton_inicio_textura, posX, posY, WHITE);
    }
    EndDrawing();
}

// Funcion para incrementar el puntaje del juego
void incrementar_puntaje(juego *juego) 
{
    juego->puntaje++;
}

// Funcion para reiniciar el puntaje del juego
void resetear_puntaje(juego *juego) 
{
    juego->puntaje = 0; 
}

// Funcion para dibujar el puntaje en la pantalla del juego
void dibujar_puntaje(juego *juego) 
{
    char textoPuntaje[20];
    sprintf(textoPuntaje, "Puntaje: %d", juego->puntaje);
    DrawText(textoPuntaje, 10, 10, 20, BLACK);
}

// Funcion para actualizar el estado del juego
void actualizar(juego *juego) 
{
    if (juego->empezado) 
    {
        // Logica de actualizacion del juego
        bool arriba = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP),
            abajo = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN),
            izquierda = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT),
            derecha = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

        if (juego->frames_transcurridos % (FPS / TPS) == 0) 
        {
            // Actualiza la direccion de la serpiente segun las teclas presionadas
            if (arriba && (juego->dx == -1 || juego->dx == 1)) 
            {
                juego->dx = 0;
                juego->dy = -1;
            } 
            else if (abajo && (juego->dx == -1 || juego->dx == 1)) 
            {
                juego->dx = 0;
                juego->dy = 1;
            } 
            else if (izquierda && (juego->dy == -1 || juego->dy == 1)) 
            {
                juego->dy = 0;
                juego->dx = -1;
            } 
            else if (derecha && (juego->dy == -1 || juego->dy == 1)) 
            {
                juego->dy = 0;
                juego->dx = 1;
            }

            // Crea un nuevo nodo para la serpiente
            nodo *nuevonodo = (nodo *)malloc(sizeof(nodo));
            nuevonodo->x = juego->serpiente->x + juego->dx;
            nuevonodo->y = juego->serpiente->y + juego->dy;
            nuevonodo->siguiente = juego->serpiente;
            juego->serpiente = nuevonodo;

            // Incrementa el tamano de la serpiente
            juego->tamano++;

            // Verifica si la serpiente come la manzana
            if (juego->serpiente->x == juego->manzana_x && juego->serpiente->y == juego->manzana_y) 
            {
                colocar_manzana(juego);
                incrementar_puntaje(juego);
            } 
            else 
            {
                // Reduce el tamano de la serpiente si no come la manzana
                nodo *actual = juego->serpiente;
                nodo *previo = NULL;
                while (actual->siguiente) 
                {
                    previo = actual;
                    actual = actual->siguiente;
                }
                previo->siguiente = NULL;
                juego->tamano--;
            }

            // Verifica si la serpiente ha llenado todo el tablero
            if (juego->tamano == ANCHO_CELDA * ALTO_CELDA) 
            {
                juego->juego_terminado = 1;
            }

            // Verifica si la serpiente ha chocado con los bordes del tablero
            if (juego->serpiente->x < 0 || juego->serpiente->y < 0 || juego->serpiente->x >= ANCHO_CELDA || juego->serpiente->y >= ALTO_CELDA) 
            {
                juego->juego_terminado = 1;
                resetear_puntaje(juego);
            }

            // Verifica si la serpiente ha chocado consigo misma
            {
                nodo *actual = juego->serpiente->siguiente;
                while (actual) 
                {
                    if (actual->x == juego->serpiente->x && actual->y == juego->serpiente->y) 
                    {
                        juego->juego_terminado = 1;
                        resetear_puntaje(juego);
                        break;
                    }
                    actual = actual->siguiente;
                }
            }
        }
        juego->frames_transcurridos++;
    }
    else 
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
        {
            juego->empezado = true;
        }
    }
}