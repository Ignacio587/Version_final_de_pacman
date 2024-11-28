#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H
void BuscarPacman(int** matriz, int nFilas, int nColumnas, int* x, int* y);
void MoverJugador(int** matriz, int nFilas, int nColumnas, int direccion, int* puntaje);
void BuscarFantasma(int** matriz, int nFilas, int nColumnas, int* x, int* y, int FANTASMA);
void MoverFantasma(int** matriz, int nFilas, int nColumnas, int FANTASMA, int *encima);
#endif