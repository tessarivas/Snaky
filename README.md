# Snaky en C con Lista Enlazada Simple

**Snaky** es un juego de la serpiente simple implementado en C usando la librería [raylib](https://www.raylib.com/). El juego presenta una interfaz gráfica donde el jugador controla una serpiente que debe comer manzanas para crecer y evitar colisiones.

## Información del Programa

**Nombre:** Teresa Rivas Gómez  
**Matrícula:** 372565  
**Materia:** Paradigmas de la Programación  
**Programa:** Juego de la serpiente  
**Fecha de Inicio:** 10/04/2024  
**Fecha de Final:** 23/05/2024  

## Características

- Gráficos sencillos y coloridos.
- Control del juego usando el teclado.
- Detección de colisiones y reinicio automático del juego.
- Puntuación en tiempo real.

## Requisitos

- [raylib](https://www.raylib.com/) 4.0 o superior.
- Compilador C compatible.

## Instalación

1. Clona el repositorio:

    ```sh
    git clone https://github.com/tessarivas/Paradigmas-de-Programacion.git
    cd Paradigmas-de-Programacion
    ```

2. Asegúrate de tener raylib instalada en tu sistema. Puedes instalarla siguiendo las instrucciones en la [página oficial de raylib](https://www.raylib.com/).

3. Compila el juego:

    ```sh
    gcc -o snaky main.c -lraylib -lm -lpthread -ldl -lrt -lX11
    ```
    Ejecuta el juego desde la línea de comandos:
    ```sh
    ./snaky
    ```
    O desde la terminal de **w64devkit**, que se encuentra en una carpeta con el mismo nombre al instalar raylib puedes posicionarte en la carpeta **build** de este programa y utilizar los comandos:

    ```sh
    make
    ```
    ```sh
    ./Game.exe
    ```
