#ifndef MAPA_H
#define MAPA_H

#include "tiposDatos.h"

void generarMapa(int[][30], int);
void generarCuartos(int, int, posicion);
returnVecinos *todosLosVecinos(int [][30], cuarto, int);
posicion generarPrimerCuarto(int [][30], int);
int posicionValida(cuarto cuartos[], posicion, int);


// Función para inicializar los valores de la matriz mapa a 1
// params:
//  int max puede ser 10, 20, 30 dependiendo de la dificultad
void generarMapa(int mapa[][30], int max) {
  posicion cuartoActual;
  cuartoActual = generarPrimerCuarto(mapa, max);
  generarCuartos(1, max, cuartoActual);
}

void generarCuartos(int posicionActual, int max, posicion pos) {
  if (posicionActual == max - 1) {
    // Liberar memoria de vecinosFactibles
    return;
  }
  posicionActual = posicionActual + 1;
  generarCuartos(posicionActual, max, pos);
}
/*
[2,3]

[0,0][0,1][0,2][0,3][0,4][0,5]
[1,0][1,1][1,2][1,3][1,4][1,5]
[2,0][2,1][2,2][2,3][2,4][2,5]
[3,0][3,1][3,2][3,3][3,4][3,5]
[4,0][4,1][4,2][4,3][4,4][4,5]

*/
returnVecinos *todosLosVecinos(int mapa[][30], cuarto cuartoActual, int max) {
  int cantidadVecinosFactible = 0;
  posicion vecinos[max];
  posicion pos;

  if (cuartoActual.pos.x != 0) {
    if (mapa[cuartoActual.pos.x - 1][cuartoActual.pos.y] == 0) {
      pos.x = cuartoActual.pos.x - 1;
      pos.y = cuartoActual.pos.y;
      vecinos[cantidadVecinosFactible] = pos;
      cantidadVecinosFactible++;
    }
  }

  if (cuartoActual.pos.y != 0) {
    if (mapa[cuartoActual.pos.x][cuartoActual.pos.y - 1] == 0) {
      pos.x = cuartoActual.pos.x;
      pos.y = cuartoActual.pos.y - 1;
      vecinos[cantidadVecinosFactible] = pos;
      cantidadVecinosFactible++;
    }
  }

  if (cuartoActual.pos.x != max) {
    if (mapa[cuartoActual.pos.x + 1][cuartoActual.pos.y] == 0) {
      pos.x = cuartoActual.pos.x + 1;
      pos.y = cuartoActual.pos.y;
      vecinos[cantidadVecinosFactible] = pos;
      cantidadVecinosFactible++;
    }
  }

  if (cuartoActual.pos.y != max) {
    if (mapa[cuartoActual.pos.x][cuartoActual.pos.y + 1] == 0) {
      pos.x = cuartoActual.pos.x;
      pos.y = cuartoActual.pos.y + 1;
      vecinos[cantidadVecinosFactible] = pos;
      cantidadVecinosFactible++;
    }
  }
  returnVecinos *ret = malloc(sizeof(returnVecinos) + 8*4);

  for (int n = 0; n < cantidadVecinosFactible; n++) {
    ret->pos[n] = vecinos[n];
  }
  ret->cantidad = cantidadVecinosFactible;
  return ret;
} // Termina todosLosVecinos

// Función para crear el primer cuarto
// params:
//  int max puede ser 10, 20, 30 dependiendo de la dificultad
posicion generarPrimerCuarto(int mapa[][30],int max) {
  posicion inicial;
  inicial.x = rand() % max;;
  inicial.y = rand() % max;
  mapa[inicial.x][inicial.y] = 1;
  return inicial;
}

//Funcion que recibe una posicion y el total numero de cuartos y decide si
//en la posicion puede existir un cuarto o no
int posicionValida(cuarto cuartos[], posicion posicion, int max) {
  // faltan vlaidaciones
  for (int i = 0; i < max; i++) {
     printf("Cuarto %i ,X=%i y Y=%i \n", i,cuartos[i].pos.x, cuartos[i].pos.y);
    if ((cuartos[i].pos.x+1 == (posicion.x) ||
        cuartos[i].pos.x-1 == (posicion.x)) && 
        cuartos[i].pos.y == posicion.y){
      if (cuartos[i].cantidadDeCuartosVecinos > 2) {
        return 0; // Significa que los vecinos ya tienen el maximo de vecinos
      }
    }
    if ((cuartos[i].pos.y-1 == (posicion.y) ||
        cuartos[i].pos.y+1 == (posicion.y)) &&
        cuartos[i].pos.x == posicion.x){
      if (cuartos[i].cantidadDeCuartosVecinos > 2) {
        
        return 0; // Significa que los vecinos ya tienen el maximo de vecinos
      }
    }
  }
  return 1; // Signfica que a los vecinos se les puede agregar un cuarto mas
}

#endif