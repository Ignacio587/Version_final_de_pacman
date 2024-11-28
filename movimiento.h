#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

// Función que permite buscar a Pacman en la matriz de juego
void BuscarPacman(int** matriz, int nFilas, int nColumnas, int* x, int* y);

// Función que encuentra a Pacman y lo mueve según los parametros.
// También maneja el puntaje, cada vez que pacman se come un punto, el puntaje aumenta.
void MoverJugador(int** matriz, int nFilas, int nColumnas, int direccion, int* puntaje);

// Función que permite buscar fantasmas en la matriz de juego
void BuscarFantasma(int** matriz, int nFilas, int nColumnas, int* x, int* y, int FANTASMA);

// Función que permite mover a los fantasmas en la matriz.
// Detecta si la posición en la que estaba tenía un punto o no, y así no elimina puntos al pasar por ellos.
// También se pueden comer a Pacman, y al hacerlo pacman será enviado a la casilla "muerto" (-5, -5) donde
//  se le restará una vida y todos serán enviados a su posición de inicio.
void MoverFantasma(int** matriz, int nFilas, int nColumnas, int FANTASMA, int *encima);

#endif