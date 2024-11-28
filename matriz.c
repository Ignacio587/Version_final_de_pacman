// matriz.c
#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"

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