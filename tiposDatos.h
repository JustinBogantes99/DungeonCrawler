#ifndef DATATYPES_H
#define DATATYPES_H

typedef struct {
  int x;
  int y;
} posicion;

// Tipo 0: Cuarto inicio, Tipo 1: Cuarto normal, Tipo 2: Cuarto final
typedef struct {
  posicion pos;
  int tipoCuarto;
  int hayMonstruo;
  int cantidadDeCuartosVecinos;
} cuarto;

// Tipo 0: Heroe, Tipo 1: Monstruo
//estado 0: inactivo, estado 1: activo, estado 2: espera
typedef struct {
  posicion pos;
  int estado;
  int tipo;
  int vida;
  int ataque;
} personaje;

// Tipo 0: Trampa, Tipo 1: Tesoro
typedef struct {
  posicion pos;
  int tipoObjeto;
  int vida;
  int ataque;
  int activo;

} objeto;

typedef struct {
  int cantidad;
  posicion pos[];
} returnVecinos;

#endif
