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
int vidas = 3;

// Para comprobar si hay puntos en los fantasmas
int encimaBLYNKY=0, encimaPINKY=0, encimaINKY=0, encimaCLYDE=0;
int encimabarreraBLYNKY = 0, encimaBarreraPINKY = 0, encimabarreraINKY=0, encimaencimaINKY=0, encimabarreraCLYDE=0;


// Configurar terminal en modo raw (capturar teclas sin Enter)
void enableRawMode() {
    struct termios termiosOriginal;
    tcgetattr(STDIN_FILENO, &termiosOriginal);
    struct termios raw = termiosOriginal;

    raw.c_lflag &= ~(ICANON | ECHO);           // Desactiva modo canónico y eco
    raw.c_cc[VMIN] = 1;                       // Procesar entrada carácter por carácter
    raw.c_cc[VTIME] = 0;                      // Sin tiempo de espera

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
// Restaurar terminal al modo normal
void disableRawMode()
{
    struct termios termiosOriginal;
    tcgetattr(STDIN_FILENO, &termiosOriginal); // Obtener configuración actual
    termiosOriginal.c_lflag |= (ICANON | ECHO); // Reactivar modo canónico y eco
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosOriginal); // Aplicar cambios
}


// Leer posiciones de pacman y fantasmas desde archivo posicionesTxt



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
        
        // Configurar el tiempo de espera 0.5 segundos 
    
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
    
    
    
    LiberarMatriz(matrizPrincipal, nFilas);

    return 0;
}
