// CODIGO ANTERIOR CON VARIABLES GLOBALES //

// BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

// DEFINICIONES
#define ANCHO 800
#define ALTO 800
#define FPS 60
#define TPS 7 // Definicion de actualizaciones por segundo (ajusta la dificultad)
#define TAM_CELDA 38
#define ANCHO_CELDA 19
#define ALTO_CELDA 19
#define RAIZ_2 1.41421356237
#define TAM_MAX ANCHO_CELDA * ALTO_CELDA

// TITULO Y COLORES
const char TITULO[] = "Snaky";
const Color COLOR_FONDO = {255, 235, 39, 255};
const Color COLOR_TABLERO_1 = {40, 121, 0, 255};
const Color COLOR_TABLERO_2 = {51, 153, 0, 255};
const Color COLOR_SERPIENTE = {255, 0, 204, 255};
const Color OSCURO = {0, 0, 0, 100};

// TEXTURAS
Texture2D manzanaTextura;
Texture2D botonInicioTextura;

// VARIABLES GLOBALES
unsigned frames_transcurridos;
bool juego_terminado;
bool empezado;
Nodo *serpiente;
int manzana_x, manzana_y, dx, dy, tamano;
int puntaje;

// ESTRUCTURA DEL NODO
typedef struct Nodo {
    int x, y; // Coordenadas del nodo
    Nodo *siguiente; // Puntero al siguiente nodo
}Nodo;

// FUNCION PARA COLOCAR LA MANZANA EN UNA POSICION ALEATORIA EN EL TABLERO
void ColocarManzana() 
{
    while (1) 
    {
        int rx = rand() % ANCHO_CELDA;
        int ry = rand() % ALTO_CELDA;

        Nodo *actual = serpiente;
        while (actual) 
        {
            // VERIFICAR SI LA POSICION NO ESTA OCUPADA YA POR LA SERPIENTE
            if (actual->x == rx && actual->y == ry) 
            {
                break;
            }
            actual = actual->siguiente;
        }
        // SI NO SE ENCONTRO NINGUN CONFLICO PROCEDE A ASIGNARSE A LA VARIABLE
        if (!actual) 
        {
            manzana_x = rx;
            manzana_y = ry;
            break;
        }
    }
}

// FUNCION PARA REINICIAR EL JUEGO: RESTABLECE LOS VALORES INICIALES
void Reiniciar() 
{
    // REINICIA TODO A 
    frames_transcurridos = 0;
    juego_terminado = 0;
    tamano = 0;
    // ESTABLECE LAS DIRECCIONES INICIALES
    dx = 1;
    dy = 0;
    // CREA LA SERPIENTE INICIAL CON 3 NODOS
    serpiente = (Nodo *)malloc(sizeof(Nodo));
    serpiente->x = 7;
    serpiente->y = 9;
    serpiente->siguiente = (Nodo *)malloc(sizeof(Nodo));
    serpiente->siguiente->x = 6;
    serpiente->siguiente->y = 9;
    serpiente->siguiente->siguiente = (Nodo *)malloc(sizeof(Nodo));
    serpiente->siguiente->siguiente->x = 5;
    serpiente->siguiente->siguiente->y = 9;
    serpiente->siguiente->siguiente->siguiente = NULL;
    // COLOCA LA MANZANA EN UNA POSICION ALEATORIA
    ColocarManzana();
}

// FUNCION PARA DIBUJAR EL TABLERO, LA SERPIENTE Y LA MANZANA
void Dibujar() 
{
    BeginDrawing();
    ClearBackground(COLOR_FONDO);
    // DIBUJA EL TABLERO CON LAS CELDAS ALTERNADAS ENTRE 2 COLORES
    for (int i = 0; i < ALTO_CELDA; i++) 
    {
        for (int j = 0; j < ANCHO_CELDA; j++) 
        {
            DrawRectangle((j + 1) * TAM_CELDA, (i + 1) * TAM_CELDA, TAM_CELDA, TAM_CELDA,
                          i * ANCHO_CELDA + j & 1 ? COLOR_TABLERO_2 : COLOR_TABLERO_1);
        }
    }
    // DIBUJA LA SERPIENTE
    {
        Nodo *actual = serpiente;
        while (actual) 
        {
            DrawRectangle((actual->x + 1) * TAM_CELDA, (actual->y + 1) * TAM_CELDA, TAM_CELDA,
                          TAM_CELDA, COLOR_SERPIENTE);
            actual = actual->siguiente;
        }
    }
    // DIBUJA LA MANZANA
    DrawTextureEx(manzanaTextura, (Vector2){ (manzana_x + 1) * TAM_CELDA, (manzana_y + 1) * TAM_CELDA }, 0.0f, 1.0f, WHITE);
    // SI EL JUEGO NO HA INICIADO MUESTRA EL SIGNO DE PLAY COMO BOTON PARA INICIAR
    if (!empezado) 
    {
        DrawRectangle(0, 0, ANCHO, ALTO, OSCURO);

        Vector2 v1 = {ANCHO / 2 - RAIZ_2 * TAM_CELDA, ALTO / 2 - RAIZ_2 * TAM_CELDA};
        Vector2 v2 = {ANCHO / 2 - RAIZ_2 * TAM_CELDA, ALTO / 2 + RAIZ_2 * TAM_CELDA};
        Vector2 v3 = {ANCHO / 2 + RAIZ_2 * TAM_CELDA, ALTO / 2};

        float posX = (GetScreenWidth() - botonInicioTextura.width) / 2;
        float posY = (GetScreenHeight() - botonInicioTextura.height) / 2;

        DrawTexture(botonInicioTextura, posX, posY, WHITE);
    }
    EndDrawing();
}

// FUNCION PARA INCREMENTAR EL PUNTAJE CUANDO SE COME LA MANZANA
void IncrementarPuntaje() 
{
    puntaje++;
}

// FUNCION PARA RESETEAR EL PUNTAJE
void ResetearPuntaje() 
{
    puntaje = 0; 
}

// FUNCION PARA DIBUJAR EL PUNTAJE EN PANTALLA
void DibujarPuntaje() 
{
    char textoPuntaje[20];
    sprintf(textoPuntaje, "Puntaje: %d", puntaje);
    DrawText(textoPuntaje, 10, 10, 20, BLACK);
}

// FUNCION PARA ACTUALIZAR EL ESTADO DEL JUEGO Y MOVER LA SERPIENTE
void Actualizar() 
{
    // SI SE PRESIONA EL BOTON DE PLAY, INICIA
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
    {
        empezado = 1;
    }
    // SI NO HA INICIADO, NO REALIZA NADA
    if (!empezado) 
    {
        return;
    }
    // VERIFICA LAS TECLAS PRESIONADAS, QUE SON CON LAS QUE PUEDES MOVER LA SERPIENTE
    bool arriba = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP),
         abajo = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN),
         izquierda = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT),
         derecha = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
    // ACTUALIZA LA POSICION DE LA SERPIENTE EN INTERVALOS REGULARES
    if (frames_transcurridos % (FPS / TPS) == 0) 
    {
        if (arriba && (dx == -1 || dx == 1)) 
        {
            dx = 0;
            dy = -1;
        } 
        else if (abajo && (dx == -1 || dx == 1)) 
        {
            dx = 0;
            dy = 1;
        } 
        else if (izquierda && (dy == -1 || dy == 1)) 
        {
            dy = 0;
            dx = -1;
        } 
        else if (derecha && (dy == -1 || dy == 1)) 
        {
            dy = 0;
            dx = 1;
        }
        // CREA UN NUEVO NODO PARA LA SERPIENTE
        {
            Nodo *nuevoNodo = (Nodo *)malloc(sizeof(Nodo));
            // CALCULA LA NUEVA POSICION
            nuevoNodo->x = serpiente->x + dx;
            nuevoNodo->y = serpiente->y + dy;
            // ASIGNA EL SIGUIENTE NODO AL ACTUAL
            nuevoNodo->siguiente = serpiente;
            // ESTABLECE EL NUEVO NODO COMO LA CABEZA DE LA SERPIENTE
            serpiente = nuevoNodo;
        }
        // INCREMENTA EL TAMANO DE LA SERPIENTE
        tamano++;
        // SI LA SERPIENTE COME LA MANZANA, COLOCA UNA NUEVA
        if (serpiente->x == manzana_x && serpiente->y == manzana_y) 
        {
            ColocarManzana();
            IncrementarPuntaje();
        } 
        // SI NO SE LA COME
        else 
        {
            Nodo *actual = serpiente;
            Nodo *previo = NULL;
            while (actual->siguiente) 
            {
                previo = actual;
                actual = actual->siguiente;
            }
            previo->siguiente = NULL;
            tamano--;
        }
        // SI LA SERPIENTE LLENA TODO EL TABLERO
        if (tamano == ANCHO_CELDA * ALTO_CELDA) 
        {
            juego_terminado = 1;
        }
        // SI LA SERPIENTE CHOCA CON LOS BORDES DEL TABLERO
        if (serpiente->x < 0 || serpiente->y < 0 || serpiente->x >= ANCHO_CELDA || serpiente->y >= ALTO_CELDA) 
        {
            juego_terminado = 1;
            ResetearPuntaje();
        }
        // SI LA SERPIETNE CHOCA CON ELLA MISMA
        {
            Nodo *actual = serpiente->siguiente;
            while (actual) 
            {
                if (actual->x == serpiente->x && actual->y == serpiente->y) 
                {
                    juego_terminado = 1;
                    ResetearPuntaje();
                    break;
                }
                actual = actual->siguiente;
            }
        }
    }
    // INCREMENTA EL CONTADOR DE FOTOGRAMAS TRANSCURRIDOS
    frames_transcurridos++;
}

// FUNCION PRINCIPAL DEL PROGRAMA
int main() 
{
    // INICIALIZA SEMILLA DE NUMEROS RANDOM
    srand((unsigned)time(NULL));
    // QUE NO HAYA VENTANAS PREVIAMENTE ABIERTAS
    assert(!GetWindowHandle());
    // TASA DE ACTUALIZACION DE LA VENTANA
    SetTargetFPS(60);

    // INICIALIZA LA VENTANA 
    InitWindow(ANCHO, ALTO, TITULO);
    Image icono = LoadImage("resources/SG_LogoJuego.png");
    SetWindowIcon(icono);

    // CARGAR IMAGEN MANZANA
    Image manzanaImagen = LoadImage("resources/SG_Manzana.png");
    manzanaTextura = LoadTextureFromImage(manzanaImagen);
    UnloadImage(manzanaImagen);

    Image botonInicio = LoadImage("resources/SG_BotonInicio.png");
    botonInicioTextura = LoadTextureFromImage(botonInicio);
    UnloadImage(botonInicio);

    // REINICIA EL JUEGO
    Reiniciar();

    // INICIALIZAR PUNTAJE
    puntaje = 0;

    // BUCLE PRINCIPAL
    while (!WindowShouldClose()) 
    {
        // SI EL JUEGO A TERMINADO
        if (juego_terminado) 
        {
            // ESPERA 1 SEGUNDO PARA REINICIAR EL JUEGO
            sleep(1);
            Reiniciar();
            continue;
        }
        // ACTUALIZA EL ESTADO DEL JUEGO Y POSICIONES
        Actualizar();
        // DIBUJA EL TABLERO, SERPIENTE Y MANZANA
        Dibujar();
        // DIBUJA EL PUNTAJE
        DibujarPuntaje();
    }
    // SE ASEGURA QUE HAYA UNA VENTANA ABIERTA
    assert(GetWindowHandle());
    CloseWindow();
    // DES-CARGAR IMAGENES
    UnloadTexture(manzanaTextura);
    UnloadTexture(botonInicioTextura);
    return 0;
} 