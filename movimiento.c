#include "pacman.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

#include <sys/select.h> // Biblioteca para movimiento continuo.
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
void BuscarPacman(int** matriz, int nFilas, int nColumnas, int* x, int* y)
{
    for(int i = 0; i < nFilas; i++)
    {
        for(int j = 0; j < nColumnas; j++)
        {
            if(matriz[i][j] == PACMAN){
                *x = i;
                *y = j;   
                return;     
            }
            else{
                *x = muerto;
                *y = muerto;
               
            }
        }
    }
}
void MoverJugador(int** matriz, int nFilas, int nColumnas, int direccion, int* puntaje)
{
    
    int xPacman, yPacman;
    BuscarPacman(matriz, nFilas, nColumnas, &xPacman, &yPacman);
    
    // Determinar nueva posición
    int newX = xPacman, newY = yPacman;
    if(direccion == UP){
        newX--;
        if(newX==-1) newX=nFilas-1;
    }
    else if(direccion == DOWN){
        newX++;
        if(newX==nFilas) newX=0;
    }
    else if(direccion == LEFT){
        newY--;
        if(newY==-1) newY=nColumnas-1;
    }
    else if(direccion == RIGHT){
        newY++;
        if(newY==nColumnas) newY=0;
    }

    // Validar movimiento
    if (newX >= 0 && newX < nFilas && newY >= 0 && newY < nColumnas)
    {
        if(matriz[newX][newY] == VACIO){
            matriz[xPacman][yPacman] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = PACMAN;     // Mover Pac-Man
        }
        else if(matriz[newX][newY] == PUNTO){
            matriz[xPacman][yPacman] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = PACMAN;     // Mover Pac-Man
            *puntaje+=10;       // Aumentar puntaje en 10
        }
        else if(matriz[newX][newY] == PUNTO_GRANDE){
            matriz[xPacman][yPacman] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = PACMAN;     // Mover Pac-Man
            *puntaje+=50;       // Aumentar puntaje en 50
        }
    }
}
void BuscarFantasma(int** matriz, int nFilas, int nColumnas, int* x, int* y, int FANTASMA)
{
    for(int i = 0; i < nFilas; i++)
    {
        for(int j = 0; j < nColumnas; j++)
        {
            if(matriz[i][j] == FANTASMA)
            {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}
void MoverFantasma(int** matriz, int nFilas, int nColumnas, int FANTASMA, int *encima)
{
    int xFantasma, yFantasma;

    // Encontrar FANTASMA y guardar su posición en xFantasma e yFantasma
    BuscarFantasma(matriz, nFilas, nColumnas, &xFantasma, &yFantasma, FANTASMA);


    int direccion = ((rand()%4)+1);

    // Determinar nueva posición
    int newX = xFantasma, newY = yFantasma;
    if(direccion == UP){
        newX--;
        if(newX==-1)
            newX=nFilas-1;
    }
    else if(direccion == DOWN){
        newX++;
        if(newX==nFilas)
            newX=0;
    }
    else if(direccion == LEFT){
        newY--;
        if(newY==-1)
            newY=nColumnas-1;
    }
    else if(direccion == RIGHT){
        newY++;
        if(newY==nColumnas)
            newY=0;
    }

    // Validar y mover fantasma
    if (newX >= 0 && newX < nFilas && newY >= 0 && newY < nColumnas)
    {
        if(matriz[newX][newY] == PARED){   
            MoverFantasma(matriz, nFilas, nColumnas, FANTASMA, encima); // 
        }
        else if(matriz[newX][newY]==PUNTO){ 
            if(*encima){  // Ver si el fantasma está encima de un punto o no
                matriz[xFantasma][yFantasma] = PUNTO;  // Si lo está la posición donde estaba el fantasma pasa a ser un punto
            }
            else{
                matriz[xFantasma][yFantasma] = VACIO;  // Y si no la posición pasa a ser vacío
                *encima=1;
            }
            matriz[newX][newY] = FANTASMA;     // Mover Fantasma
        }
        else if(matriz[newX][newY] == VACIO){
            if(*encima){   // La misma lógica anterior
                matriz[xFantasma][yFantasma] = PUNTO; // Dejar un punto en donde estaba el fantasma
                *encima=0;  // Como la posición si o no va estar encima
            }
            else{
                matriz[xFantasma][yFantasma] = VACIO;
                *encima=0;
            }
            matriz[newX][newY] = FANTASMA;
        }
        else if(matriz[newX][newY]==BARRERA){
            matriz[xFantasma][yFantasma] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = FANTASMA;      // Mover Fantasma 
        }
        else if (matriz[newX][newY] == BLINKY || matriz[newX][newY] == INKY || matriz[newX][newY] == PINKY || matriz[newX][newY] == CLYDE){
            MoverFantasma(matriz,nFilas,nColumnas,FANTASMA, encima);
        }
        else if( matriz[newX][newY] == PUNTO_GRANDE){
            matriz[xFantasma][yFantasma] = PUNTO_GRANDE; // Dejar un punto grande en la posición anterior
            matriz[newX][newY] = FANTASMA;      // Mover Fantasma
        }
        else if (matriz[newX][newY]== PACMAN){
            matriz[xFantasma][yFantasma]= VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = FANTASMA;      // Mover Fantasma

         }   
    }
}
// Eliminar a Pacman y a los Fantasmas de la matriz, para reiniciar sus posiciones luego de perder una vida
