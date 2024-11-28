// matriz.h
#ifndef MATRIZ_H
#define MATRIZ_H

// Función que creará una matriz dinámica de tamaño nFilas por nColumnas.
int** CrearMatriz(int nFilas, int nColumnas);

// Función que leerá el archivo mapa.txt correspondiente, y lo insertará en la matriz de juego.
void RecibirMapa(const char* filename, int** matriz, int nFilas, int nColumnas);

// Función que actualizará constantemente el archivo donde se guardará la matriz con el estado actual.
void ActualizarMapa(const char* filename, int** matriz, int nFilas, int nColumnas);

// Función para liberar el espacio asignado por la matriz, antes de finalizar el programa.
void LiberarMatriz(int** matriz, int nFilas);

#endif