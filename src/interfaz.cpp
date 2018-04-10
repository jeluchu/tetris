#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "main.h"
#include "interfaz.h"
#include "global.h"

#define DELAY 30000

Shape current;

Shape forma_copiada(Shape shape){
    Shape nueva_forma = shape;
    char **forma_copiada = shape.array;
    nueva_forma.array = (char**)malloc(nueva_forma.width*sizeof(char*));
    int i, j;
    for(i = 0; i < nueva_forma.width; i++){
        nueva_forma.array[i] = (char*)malloc(nueva_forma.width*sizeof(char));
        for(j=0; j < nueva_forma.width; j++) {
            nueva_forma.array[i][j] = forma_copiada[i][j];
        }
    }
    return nueva_forma;
}

void eliminar_formas(Shape shape){
    int i;
    for(i = 0; i < shape.width; i++){
        free(shape.array[i]);
    }
    free(shape.array);
}

int revisar_posicion(Shape shape){ //REVISA LA POSICIÓN DE LA FORMA COPIADA
    char **array = shape.array;
    int i, j;
    for(i = 0; i < shape.width;i++) {
        for(j = 0; j < shape.width ;j++){
            if((shape.col+j < 0 || shape.col+j >= COLUMNAS || shape.row+i >= FILAS)){ //FUERA DE LOS BORDES
                if(array[i][j])
                    return FALSE;
            }
            else if(Table[shape.row+i][shape.col+j] && array[i][j])
                return FALSE;
        }
    }
    return TRUE;
}

void obtener_nuevas_formas(){ //DEVUELVE UNA FORMA ALEATORIA

    Shape nueva_forma = forma_copiada(ShapesArray[rand()%7]);

    nueva_forma.col = rand()%(COLUMNAS-nueva_forma.width+1);
    nueva_forma.row = 0;
    eliminar_formas(current);
    current = nueva_forma;
    if(!revisar_posicion(current)){
        GameOn = FALSE;
    }
}

void girar_forma(Shape shape){
    Shape temp = forma_copiada(shape);
    int i, j, k, width;
    width = shape.width;
    for(i = 0; i < width ; i++){
        for(j = 0, k = width-1; j < width ; j++, k--){
                shape.array[i][j] = temp.array[k][i];
        }
    }
    eliminar_formas(temp);
}

void escribir_en_tabla(){
    int i, j;
    for(i = 0; i < current.width ;i++){
        for(j = 0; j < current.width ; j++){
            if(current.array[i][j])
                Table[current.row+i][current.col+j] = current.array[i][j];
        }
    }
}

void el_revisador(){
    int i, j, sum, count=0;
    for(i=0;i<FILAS;i++){
        sum = 0;
        for(j=0;j< COLUMNAS;j++) {
            sum+=Table[i][j];
        }
        if(sum==COLUMNAS){
            count++;
            int l, k;
            for(k = i;k >=1;k--)
                for(l=0;l<COLUMNAS;l++)
                    Table[k][l]=Table[k-1][l];
            for(l=0;l<COLUMNAS;l++)
                Table[k][l]=0;
        }
    }
    timer-=1000; puntuacion += 100*count;
}

void dibujar_tabla(){
    char Buffer[FILAS][COLUMNAS] = {0};
    int i, j;

    for(i = 0; i < current.width ;i++){
        for(j = 0; j < current.width ; j++){
            if(current.array[i][j])
                Buffer[current.row+i][current.col+j] = current.array[i][j];
        }
    }
    clear();
    for(i = 0; i < FILAS ;i++){
        for(j = 0; j < COLUMNAS ; j++){
            printw("%s", (Table[i][j] + Buffer[i][j])? "0": ".");
        }
        printw("\n");
    }

    if (has_colors())
        start_color();

    attron(A_BOLD);
    attron(COLOR_PAIR(2));
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    printw("\nPuntuación: ");
    attroff(COLOR_PAIR(2));
    attroff(A_BOLD);

    printw("%d\n", puntuacion);
}

void manipulate(int action){
    Shape temp = forma_copiada(current);
    switch(action){
        case 's':
            temp.row++;  //MOVER HACIA ABAJO CON VELOCIDAD
            if(revisar_posicion(temp))
                current.row++;
            else {
                escribir_en_tabla();
                el_revisador(); //check full lines, after putting it down
                obtener_nuevas_formas();
            }
        break;
        case 'd':
            temp.col++;  //MOVER A LA DERECHA
            if(revisar_posicion(temp))
                current.col++;
        break;
        case 'a':
            temp.col--;  //MOVER HACIA LA IZQUIERDA
            if(revisar_posicion(temp))
                current.col--;
        break;
        case 'w':
            girar_forma(temp);  //ROTACIÓN SOBRE SU EJE
            if(revisar_posicion(temp))
                girar_forma(current);
        break;
        case 'p': //PAUSA EL JUEGO HASTA LA PULSACIÓN DE CUALQUIER TECLA
            attron(A_BOLD);
            attron(COLOR_PAIR(3));
            init_pair(3, COLOR_YELLOW, COLOR_BLACK);
            mvprintw(1, 0, "PAUSA - Pulsa cualquier tecla para continuar");
            attroff(COLOR_PAIR(3));
            attroff(A_BOLD);
      			while (getch() == ERR) {
      				usleep(DELAY * 7);
      			}
        break;
    }
    eliminar_formas(temp);
    dibujar_tabla();
}
