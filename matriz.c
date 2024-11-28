// matriz.c
#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"

// Crear matriz dinámica de nFilas por nColumnas
int** CrearMatriz(int nFilas, int nColumnas) {
    int** matriz = malloc(nFilas * sizeof(int*));
    for (int i = 0; i < nFilas; i++) {
        matriz[i] = malloc(nColumnas * sizeof(int));
    }
    return matriz;
}

// Leer mapa desde archivo matrizTxt[] (filename) y recibirlo en matrizActual (matriz)
void RecibirMapa(const char* filename, int** matriz, int nFilas, int nColumnas) {
    FILE* archivo = fopen(filename, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");    // En caso no no exitir el archivo, o no poder abrirlo,
        exit(EXIT_FAILURE);                     //  se imprimirá error, y el programa dejará de ejecutarse.
    }
    for (int i = 0; i < nFilas; i++) {
        for (int j = 0; j < nColumnas; j++) {       // Acá se añade el archivo matriz.txt correspondiente a la 
            fscanf(archivo, "%d", &matriz[i][j]);   //  matriz principal de juego.
        }
    }
    fclose(archivo);    // Cerramos el archivo.
}

// Actualizamos el mapa al principio de cada loop para reflejar los cambios hechos en juego.
void ActualizarMapa(const char* filename, int** matriz, int nFilas, int nColumnas) {
    FILE* archivo = fopen(filename, "w");
    for (int i = 0; i < nFilas; i++) {
        for (int j = 0; j < nColumnas; j++) {       // Sigue la misma lógica que el anterior, sólo que esta vez se 
            fprintf(archivo, "%d ", matriz[i][j]);  //  utiliza 'w' para sobreescribir el archivo de llegada. (matriz_actual.txt)
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}

// Liberamos el espacio asignado a la matriz.
void LiberarMatriz(int** matriz, int nFilas) {
    for (int i = 0; i < nFilas; i++) {
        free(matriz[i]);        // Liberamos cada arreglo de enteros.
    }
    free(matriz);          // Y liberamos el arreglo de arreglos.
}