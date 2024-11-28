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

// Colores

// Vidas
int vidas = 2;

// Para comprobar si hay puntos en los fantasmas
int encimaBLYNKY=0, encimaPINKY=0, encimaINKY=0, encimaCLYDE=0;
int encimabarreraBLYNKY = 0, encimaBarreraPINKY = 0, encimabarreraINKY=0, encimaencimaINKY=0, encimaCLYDE=0;


// Configurar terminal en modo raw (capturar teclas sin Enter)
void enableRawMode()
{
    struct termios termiosOriginal;
    tcgetattr(STDIN_FILENO, &termiosOriginal); // Obtener configuración actual
    struct termios raw = termiosOriginal;
    raw.c_lflag &= ~(ICANON | ECHO);           // Desactivar modo canónico y eco
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // Aplicar cambios
}

// Restaurar terminal al modo normal
void disableRawMode()
{
    struct termios termiosOriginal;
    tcgetattr(STDIN_FILENO, &termiosOriginal); // Obtener configuración actual
    termiosOriginal.c_lflag |= (ICANON | ECHO); // Reactivar modo canónico y eco
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosOriginal); // Aplicar cambios
}


// Crear matriz dinámica de nFilas por nColumnas


// Leer mapa desde archivo matrizTxt[] y recibirlo en matrizActual


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

// Sobreescribir mapa actual para que quede guardado


// Imprimir la matriz

// Imprimir información 

// Imprimir pantalla final y puntaje obtenido


// Buscar la posición de Pac-Man


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

// Mover a Pac-Man en la matriz


// Buscar la posición de los fantasmas


// Mover a los Fantasmas en la matriz


// Escoger uno de los 6 mapas disponibles
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


// Usar select() para entrada no bloqueante

int main() {
    // Tamaño de la matriz
    int nFilas = 31, nColumnas = 28;

    // Escoger un mapa
    char mapa[32], posiciones[32];

    EscogerMapa(mapa, posiciones);

    // Crear matriz e inicializar con mapa inicial
    int** matrizPrincipal = CrearMatriz(nFilas, nColumnas);
    RecibirMapa(mapa, matrizPrincipal, nFilas, nColumnas);

    // Posición inicial Pacman y fantasmas
    RecibirPosiciones(posiciones, matrizPrincipal);

    /* SOLO LINUX */
    // Configurar terminal en modo raw para recibir input directo*/
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
        // system("cls"); // Limpiar pantalla WINDOWS
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
        // Hasta aquí termina la implementacíon de la funcion 

        // char input = _getch(); // Capturar tecla WINDOWS
        int direccion = 0;
        
        switch (input) {
            case 'w': direccion = UP; break;
            case 'a': direccion = LEFT; break;
            case 's': direccion = DOWN; break;
            case 'd': direccion = RIGHT; break;
            case 'q': running = false; break; // Salir del juego
        }


        if (direccion) {
            MoverJugador(matrizPrincipal, nFilas, nColumnas, direccion, &puntaje);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, BLINKY, &encimaBLYNKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, PINKY, &encimaPINKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, INKY, &encimaINKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, CLYDE, &encimaCLYDE);
            tiempo++;
        }
        int xPacman, yPacman;
        BuscarPacman(matrizPrincipal, nFilas, nColumnas, &xPacman, &yPacman);

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

    /* SOLO LINUX
    // Restaurar terminal al modo normal*/
    disableRawMode();
    enableKeyRepeat();
    
    
    LiberarMatriz(matrizPrincipal, nFilas);

    return 0;
}