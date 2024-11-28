#include <stdio.h>

#include <stdlib.h>


#include <ctype.h>
#include <time.h>
#include <locale.h>

#include <sys/select.h> // Biblioteca para movimiento continuo.
#include <termios.h>
#include <unistd.h>
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
void ImprimirMatriz(int** matriz, int nFilas, int nColumnas)
{
	for(int i=0; i<nFilas; i++)
	{
		for(int j=0; j<nColumnas; j++) 
		{
		 	switch(matriz[i][j])
			{
				case PARED:
				    printf(BLUE"██"RESET);	//	██
				    break;
				case VACIO:
				    printf("  ");
				    break;
				case PUNTO:
					printf("• ");	//	•
					break;
				case PUNTO_GRANDE:
					printf("⬤ ");	//	⬤
					break;
				case BARRERA:
					printf("--");	//	░░
					break;
				case PACMAN:
					printf(YELLOW"+ "RESET);	//	ᗧ
					break;
				case BLINKY:
					printf(RED"& "RESET);	//	ᗣ
					break;
				case PINKY:
					printf(MAGENTA"& "RESET);	//	ᗣ
					break;
				case INKY:
					printf(CYAN"& "RESET);	//	ᗣ
					break;
				case CLYDE:
					printf(ORANGE"& "RESET);	//	ᗣ
					break;
				default:
					printf("%d ", matriz[i][j]);
			}
		}
	printf("\n");
	}
}
void ImprimirInfo(int puntaje, int tiempo, int vidas)
{
    printf("\n");
    printf("====================  P A C M A N   ====================\n\n");    // Pᗣᗧ•••MᗣN
    printf("     Puntaje: %d       Tiempo: %d       Vidas: %d\n\n", puntaje, tiempo, vidas);
}

// Imprimir pantalla final y puntaje obtenido
void ImprimirGameOver(int puntaje)
{
    printf("\033[H\033[J");
    printf("\n\n");
    printf("====================  P A C M A N  =====================\n");
    printf("=######################################################=\n");
    printf("=######################################################=\n");
    printf("=#################### G A M E #########################=\n");
    printf("=##################### O V E R ########################=\n");
    printf("=######################################################=\n");
    printf("=######################################################=\n");
    printf("=################# Puntuacion: %d ###################=\n", puntaje);
    printf("=######################################################=\n");
    printf("========================================================\n");
}