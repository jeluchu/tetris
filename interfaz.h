#ifndef __INTERFAZ_H__
#define __INTERFAZ_H__

typedef struct {
    char **array;
    int width, row, col;
} Shape;
Shape current;

#ifdef __cplusplus
extern "C" {
#endif
void eliminar_formas(Shape shape);
int revisar_posicion(Shape shape);
void obtener_nuevas_formas();
void girar_forma(Shape shape);
void escribir_en_tabla();
void el_revisador();
void dibujar_tabla();
void manipulate(int action);
#ifdef __cplusplus
}
#endif

#endif
