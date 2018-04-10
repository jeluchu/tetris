#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
#include <locale.h>
#include "main.h"

char Table[FILAS][COLUMNAS] = {0};
int puntuacion = 0;
char GameOn = TRUE;
double timer = 500000;

Shape ShapesArray[7]= {
    {(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},                           //S_shape
    {(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},                           //Z_shape
    {(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //T_shape
    {(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //L_shape
    {(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //ML_shape
    {(char *[]){(char []){1,1},(char []){1,1}}, 2},                                                   //SQ_shape
    {(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4} //R_shape
};

int main() {
    system("toilet --gay -fpagga TETRIS");
    system("setterm -cursor off");
    setlocale(LC_ALL,"");
    srand(time(0));
    puntuacion = 0;
    int c;

    //system("toilet --gay -fpagga TETRIS");

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
        if (((double)after.tv_sec*1000000 + (double)after.tv_usec)-((double)before.tv_sec*1000000 + (double)before.tv_usec) > timer){
            before = after;
            manipulate('s');
        }
    }

    printw("\n¡GAME OVER!\n");

    eliminar_formas(current);
    endwin();
    system("setterm -cursor on");

    return EXIT_SUCCESS;
}
