#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tiposDatos.h"
#include "mapa.h"

#define PROBABILIDAD_TESORO 30
#define PROBABILIDAD_TRAMPA 40

int mapa[30][30] = {{0}};
int dificultad;
cuarto cuartos[30];
personaje monstruos[15];
objeto objetos[30];
personaje heroe;

/*
[2,3]

[0,0][0,1][0,2][0,3][0,4][0,5]
[1,0][1,1][1,2][1,3][1,4][1,5]
[2,0][2,1][2,2][2,3][2,4][2,5]
[3,0][3,1][3,2][3,3][3,4][3,5]
[4,0][4,1][4,2][4,3][4,4][4,5]

*/

// Inicio del programa
// params:
//  void
int main(void) {
  srand(time(NULL));
  cuarto test;
  test.pos.x = 0;
  test.pos.y = 0;
  returnVecinos *vecinos = todosLosVecinos(mapa, test, 10);
  for (int i = 0; i < vecinos->cantidad; i++) {
    printf("Valor de X: %d   Valor de Y: %d", vecinos->pos[i].x, vecinos->pos[i].y);
    printf("\n");
  }
  printf("\n\n");

  cuarto cuarto1;
  cuarto1.pos.x = 1;
  cuarto1.pos.y = 0;
  cuarto1.cantidadDeCuartosVecinos = 1;

  cuarto cuarto2;
  cuarto2.pos.x = 1;
  cuarto2.pos.y = 1;
  cuarto2.cantidadDeCuartosVecinos = 3;

  cuarto cuarto3;
  cuarto3.pos.x = 1;
  cuarto3.pos.y = 2;
  cuarto3.cantidadDeCuartosVecinos = 1;

  cuarto cuarto4;
  cuarto4.pos.x = 2;
  cuarto4.pos.y = 1;
  cuarto4.cantidadDeCuartosVecinos = 2;

  cuarto cuarto5;
  cuarto5.pos.x = 3;
  cuarto5.pos.y = 1;
  cuarto5.cantidadDeCuartosVecinos = 2;

  cuarto cuarto6;
  cuarto6.pos.x = 3;
  cuarto6.pos.y = 2;
  cuarto6.cantidadDeCuartosVecinos = 2;

  cuarto cuarto7;
  cuarto7.pos.x = 4;
  cuarto7.pos.y = 1;
  cuarto7.cantidadDeCuartosVecinos = 2;

  cuarto cuarto8;
  cuarto8.pos.x = 4;
  cuarto8.pos.y = 2;
  cuarto8.cantidadDeCuartosVecinos = 3;

  cuarto cuarto9;
  cuarto9.pos.x = 4;
  cuarto9.pos.y = 3;
  cuarto9.cantidadDeCuartosVecinos = 2;

  cuarto cuarto10;
  cuarto10.pos.x = 5;
  cuarto10.pos.y = 3;
  cuarto10.cantidadDeCuartosVecinos = 1;

  cuarto cuartosTest[10];

  cuartos[0] = cuarto1;
  cuartos[1] = cuarto2;
  cuartos[2] = cuarto3;
  cuartos[3] = cuarto4;
  cuartos[4] = cuarto5;
  cuartos[5] = cuarto6;
  cuartos[6] = cuarto7;
  cuartos[7] = cuarto8;
  cuartos[8] = cuarto9;
  cuartos[9] = cuarto10;

  posicion posTest;
  posTest.x = 2;
  posTest.y = 2;

  int num = posicionValida(cuartos, posTest, 10);
  printf("%i", num);
  
  free(vecinos);
  return 0;
}
