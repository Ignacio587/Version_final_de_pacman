// matriz.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matriz.h"
#include "pacman.h"

int** CrearMatriz(int nFilas, int nColumnas) {
    int** matriz = malloc(nFilas * sizeof(int*));
    for (int i = 0; i < nFilas; i++) {
        matriz[i] = malloc(nColumnas * sizeof(int));
    }
    return matriz;
}

void RecibirMapa(const char* filename, int** matriz, int nFilas, int nColumnas) {
    FILE* archivo = fopen(filename, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nFilas; i++) {
        for (int j = 0; j < nColumnas; j++) {
            fscanf(archivo, "%d", &matriz[i][j]);
        }
    }
    fclose(archivo);
}

void ActualizarMapa(const char* filename, int** matriz, int nFilas, int nColumnas) {
    FILE* archivo = fopen(filename, "w");
    for (int i = 0; i < nFilas; i++) {
        for (int j = 0; j < nColumnas; j++) {
            fprintf(archivo, "%d ", matriz[i][j]);
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}

void LiberarMatriz(int** matriz, int nFilas) {
    for (int i = 0; i < nFilas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}
void RecibirPosiciones(char posicionesTxt[], int** matrizActual)
{
    FILE* archivo = fopen(posicionesTxt, "r");
    if(archivo == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s\n", posicionesTxt);
        exit(1);
    }

    for(int i=0; i<5; i++)
    {
        int x, y;
        fscanf(archivo, "%d", &x);
        fscanf(archivo, "%d", &y);

        switch(i) {
        case 0: matrizActual[x][y] = PACMAN; break;
        case 1: matrizActual[x][y] = BLINKY; break;
        case 2: matrizActual[x][y] = PINKY; break;
        case 3: matrizActual[x][y] = INKY; break;
        case 4: matrizActual[x][y] = CLYDE; break;
        default: matrizActual[x][y] = VACIO; break;
        }
    }
    fclose(archivo);
}
void EscogerMapa(char mapa[], char posiciones[]) {
    printf("Pacman!\n\n");
    printf("Mapas disponibles:\n");
    printf("(1) Pac-Man\n");
    printf("(2) Original Prototype\n");
    printf("(3) Ms. Pac-Man 1\n");
    printf("(4) Ms. Pac-Man 2\n");
    printf("(5) Ms. Pac-Man 3\n");
    printf("(6) Ms. Pac-Man 4\n");

    // Recibir input del usuario
    char input = getchar();

    // Variables para los nombres de archivo
    const char *auxMapa = NULL;
    const char *auxPos = NULL;

    // Definir mapa y posiciones según el númeo que escoja el usuario
    switch (input) {
        case '1': 
            auxMapa = "mapa1.txt"; 
            auxPos = "posiciones1.txt"; 
            break;
        case '2': 
            auxMapa = "mapa2.txt"; 
            auxPos = "posiciones2.txt"; 
            break;
        case '3': 
            auxMapa = "mapa3.txt"; 
            auxPos = "posiciones3.txt"; 
            break;
        case '4': 
            auxMapa = "mapa4.txt"; 
            auxPos = "posiciones4.txt"; 
            break;
        case '5': 
            auxMapa = "mapa5.txt"; 
            auxPos = "posiciones5.txt"; 
            break;
        case '6': 
            auxMapa = "mapa6.txt"; 
            auxPos = "posiciones6.txt"; 
            break;
        default: 
            printf("Mapa inválido, inténtelo otra vez.\n");
            exit(0);
    }

    // Pasar los strings con los punteros que apuntan a los strings fuera de la función.
    strcpy(mapa, auxMapa);
    strcpy(posiciones, auxPos);
}
