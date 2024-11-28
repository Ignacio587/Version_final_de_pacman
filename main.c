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
//bibliotecas locales
#include "matriz.h"
#include "imprimir.h"
#include "pacman.h"
#include "movimiento.h"


// Vidas
int vidas = 3;

// Variables para comprobar si hay puntos en los fantasmas
int encimaBLYNKY=0, encimaPINKY=0, encimaINKY=0, encimaCLYDE=0;

// Configurar terminal en modo raw (capturar teclas sin Enter) (en linux)
void enableRawMode()
{
    struct termios termiosOriginal;
    tcgetattr(STDIN_FILENO, &termiosOriginal); // Obtener configuración actual
    struct termios raw = termiosOriginal;
    raw.c_lflag &= ~(ICANON | ECHO);           // Desactivar modo canónico y eco
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // Aplicar cambios
}

// Restaurar terminal al modo normal (en linux)
void disableRawMode()
{
    struct termios termiosOriginal;
    tcgetattr(STDIN_FILENO, &termiosOriginal); // Obtener configuración actual
    termiosOriginal.c_lflag |= (ICANON | ECHO); // Reactivar modo canónico y eco
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosOriginal); // Aplicar cambios
}

// Leer posiciones de pacman y fantasmas desde archivo posicionesTxt
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
        case 0: matrizActual[x][y] = PACMAN; break;     // Según la línea en que se encuentre
        case 1: matrizActual[x][y] = BLINKY; break;     //  se asignará Pacman o un Fantasma
        case 2: matrizActual[x][y] = PINKY; break;      //  en la posición dada por posiciones.txt
        case 3: matrizActual[x][y] = INKY; break;
        case 4: matrizActual[x][y] = CLYDE; break;
        default: matrizActual[x][y] = VACIO; break;
        }
    }
    fclose(archivo);
}


// Eliminar a Pacman y a los Fantasmas de la matriz, para reiniciar sus posiciones luego de perder una vida
void LimpiarPacmanFantasmas(int** matriz, int nFilas,int nColumnas)
{   encimaBLYNKY=0, encimaPINKY=0, encimaINKY=0, encimaCLYDE=0;
    for (int i = 0; i < nFilas; i++)
    {
        for (int j = 0; j < nColumnas; j++)
        {
            if (matriz[i][j] == PACMAN || matriz[i][j] == INKY || matriz[i][j] == PINKY || matriz[i][j] == BLINKY || matriz[i][j] == CLYDE)
            {
                matriz[i][j] = VACIO;
                
            }
        }
    }
}

// Escoger uno de los 4 mapas disponibles
void EscogerMapa(char mapa[], char posiciones[]) {
    printf("Pacman!\n\n");
    printf("Mapas disponibles:\n");
    printf("(1) Pac-Man\n");
    printf("(2) Original Prototype\n");
    printf("(3) Ms. Pac-Man 1\n");
    printf("(4) Ms. Pac-Man 2\n");

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
        default: 
            printf("Mapa inválido, inténtelo otra vez.\n");
            exit(0);
    }

    // Pasar los strings con los punteros que apuntan a los strings fuera de la función.
    strcpy(mapa, auxMapa);
    strcpy(posiciones, auxPos);
}


int main() {
    // Tamaño de la matriz
    int nFilas = 31, nColumnas = 28;

    // Escoger un mapa y guardarlo en 'mapa' y 'posiciones'
    char mapa[32], posiciones[32];
    EscogerMapa(mapa, posiciones);

    // Crear matriz e inicializar con mapa inicial (escogido anteriormente por el jugador)
    int** matrizPrincipal = CrearMatriz(nFilas, nColumnas);
    RecibirMapa(mapa, matrizPrincipal, nFilas, nColumnas);

    // Posición inicial Pacman y fantasmas
    RecibirPosiciones(posiciones, matrizPrincipal);

    // Configurar terminal en modo raw para recibir input directo
    disableKeyRepeat();
    enableRawMode();
    

    // Puntaje y tiempo inicial
    int puntaje = 0, tiempo = 0;

    char input = 'd';
    struct timeval timeout;
    fd_set readfds;

    // Configura el tiempo de espera en milisegundos (por ejemplo, 500 ms)
    int timeout_ms = 500;

    // Ciclo principal del juego
    int running = true;
    while (vidas>0)
    {
        // Actualizar mapa_actual.txt con información nueva y insertarlo en matrizPrincipal.
        ActualizarMapa("mapa_actual.txt", matrizPrincipal, nFilas, nColumnas);
        RecibirMapa("mapa_actual.txt", matrizPrincipal, nFilas, nColumnas);
  
        printf("\033[H\033[J"); // Limpiar pantalla LINUX
        ImprimirInfo(puntaje, tiempo, vidas);
        ImprimirMatriz(matrizPrincipal, nFilas, nColumnas);
        printf("\n-- Utilice WASD para moverse y Q para cerrar el juego --\n");


        /* Capturar tecla LINUX */
        // Desde aquí voy a estudiar la funcion, y la struct timeval
        // Configurar el tiempo de espera (5 segundos en este ejemplo)
    
        timeout.tv_sec = timeout_ms / 1000;               // Segundos
        timeout.tv_usec = (timeout_ms % 1000) * 1000;    // Microsegundos

        // Reiniciar los file descriptors
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        
        fflush(stdout);
        // Usar select para esperar entrada
        int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if (result == -1){
            perror("Error en select");
            exit(EXIT_FAILURE);
        }
        else if (result == 0){
        }
        else{
            // Leer el carácter si hay entrada
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                input = getchar();
                if (input == 'q') {
                    printf("Saliendo del programa...\n");
                    break;
                }
            }
        }
        // Hasta aquí termina la implementacíon de la función 

        int direccion = 0;
        
        switch (input) {
            case 'w': direccion = UP; break;
            case 'a': direccion = LEFT; break;
            case 's': direccion = DOWN; break;
            case 'd': direccion = RIGHT; break;
            case 'q': running = false; break; // Salir del juego
        }

        // Acá se recibe la dirección, y llamamos a las funciones de movimiento para mover a Pacman y a los fantasmas.
        if (direccion) {
            MoverJugador(matrizPrincipal, nFilas, nColumnas, direccion, &puntaje);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, BLINKY, &encimaBLYNKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, PINKY, &encimaPINKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, INKY, &encimaINKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, CLYDE, &encimaCLYDE);
            tiempo++;
        }

        // Buscamos a Pacman al final de cada ciclo, para ver si se murió
        int xPacman, yPacman;
        BuscarPacman(matrizPrincipal, nFilas, nColumnas, &xPacman, &yPacman);

        // Si está muerto, reducimos en 1 la cantidad de vidas, limpiamos el mapa y reiniciamos las posiciones.
        // En caso de que no queden vidas (vidas == 0), se imprime el game over.
        if(xPacman == muerto && yPacman == muerto){
            vidas--;
            LimpiarPacmanFantasmas(matrizPrincipal,nFilas,nColumnas);
            RecibirPosiciones(posiciones, matrizPrincipal);
            if(vidas==0){
                ImprimirGameOver(puntaje);
            }
        }
    }
    getchar(); 

    // Restaurar terminal al modo normal
    disableRawMode();
    enableKeyRepeat();
    
    // Finalmente, liberamos el espacio asignado por las matrices.
    LiberarMatriz(matrizPrincipal, nFilas);

    return 0;
}
