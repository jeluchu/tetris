#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#include "interfaz.h"

#define FILAS 20
#define COLUMNAS 11
#define TRUE 1
#define FALSE 0

char Table[FILAS][COLUMNAS] = {0};
int puntuacion = 0;
char GameOn = TRUE;
double timer = 500000;

typedef struct {
    char **array;
    int width, row, col;
} Shape;
Shape current;

const Shape ShapesArray[7]= {
    {(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},                           //S_shape
    {(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},                           //Z_shape
    {(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //T_shape
    {(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //L_shape
    {(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //ML_shape
    {(char *[]){(char []){1,1},(char []){1,1}}, 2},                                                   //SQ_shape
    {(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4} //R_shape
};

int main() {

    srand(time(0));
    puntuacion = 0;
    int c;

    initscr();

    struct timeval before, after;

    gettimeofday(&before, NULL);

    nodelay(stdscr, TRUE);

    obtener_nuevas_formas();

    dibujar_tabla();

    while(GameOn){
        if ((c = getch()) != ERR) {
          manipulate(c);
        }
        gettimeofday(&after, NULL);
        if (((double)after.tv_sec*1000000 + (double)after.tv_usec)-((double)before.tv_sec*1000000 + (double)before.tv_usec) > timer){ //time difference in microsec accuracy
            before = after;
            manipulate('s');
        }
    }

    printw("\nGame over\n");

    eliminar_formas(current);
    return 0;
}
