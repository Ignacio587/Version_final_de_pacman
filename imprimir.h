// imprimir.h
#ifndef IMPRIMIR_H
#define IMPRIMIR_H

// Recibe la matriz de enteros e imprime la matriz de juego con su ascii designado.
void ImprimirMatriz(int** matriz, int nFilas, int nColumnas);

// Imprime la información de juego (puntaje, tiempo y vidas restantes).
void ImprimirInfo(int puntaje, int tiempo, int vidas, int niveles);

// Imprime Gameover y el puntaje logrado cuando se terminar el juego.
void ImprimirGameOver(int puntaje);

#endif
