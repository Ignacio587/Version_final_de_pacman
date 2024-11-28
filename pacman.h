// include/pacman.h
#ifndef PACMAN_H
#define PACMAN_H

// Colores
#define YELLOW "\x1B[33m"
#define BLUE   "\x1b[34m"
#define MAGENTA "\x1B[35m"
#define RED     "\x1B[31m"
#define CYAN "\x1B[36m" 
#define NORMAL "\e[0m"
#define ORANGE  "\x1B[38;2;255;128;0m"
#define RESET "\033[0m"

// Constantes

// Movimiento
#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4

// Estados de la matriz
#define PARED -1
#define VACIO 0
#define PUNTO 1
#define PUNTO_GRANDE 11
#define PACMAN 2
#define BARRERA 3
#define muerto -5

// ID Fantasmas
#define BLINKY 51	// Rojo
#define PINKY 52	// Rosado
#define INKY 53		// Celeste
#define CLYDE 54	// Naranja


#endif // PACMAN_H
