// matriz.h
#ifndef MATRIZ_H
#define MATRIZ_H

int** CrearMatriz(int nFilas, int nColumnas);
void RecibirMapa(const char* filename, int** matriz, int nFilas, int nColumnas);
void ActualizarMapa(const char* filename, int** matriz, int nFilas, int nColumnas);
void LiberarMatriz(int** matriz, int nFilas);

#endif