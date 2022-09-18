#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>


#include "keyboardListener.h"
#include "tiposDatos.h"
#include "mapa.h"

#define PROBABILIDAD_TESORO 30
#define PROBABILIDAD_TRAMPA 40

int mapa[30][30] = {{0}};
int cantidadDeCuartosActual;
int dificultad;
cuarto cuartos[30];
personaje monstruos[15];
objeto objetos[30];
personaje heroe;
void generarMapa(int[][30], int);

// Inicio del programa
// params:
//  void
int main(void) {
  pthread_t guess;

  //assert (pthread_create (&guess, NULL, keyboard_listener, NULL) == 0);

  /*cuarto cuarto1;
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
  cuartos[9] = cuarto10;*/



  srand(time(NULL));
  cantidadDeCuartosActual = 0;

  generarMapa(mapa,10);

  /*void *result;
  pthread_join (guess, &result);

  if (result)
    printf ("You must have guessed 7.\n");*/

  return 0;
}


// Función para inicializar los valores de la matriz mapa a 1
// params:
//  int max puede ser 10, 20, 30 dependiendo de la dificultad
void generarMapa(int mapa[][30], int max) {
  posicion cuartoActual;
  cuartoActual = generarPrimerCuarto(mapa, max);
  cuarto primerCuarto;
  primerCuarto.pos = cuartoActual;
  primerCuarto.tipoCuarto = 1;
  primerCuarto.hayMonstruo = 0;
  primerCuarto.cantidadDeCuartosVecinos = 0;
  cuartos[0] = primerCuarto;
  cantidadDeCuartosActual++;
  printf("Primer Cuarto: X: %d  Y: %d \n\n", cuartos[0].pos.x, cuartos[0].pos.y);
  returnVecinos *vecinos = todosLosVecinos(mapa, cuartos[0], max);
  for(int i = 0; i < vecinos->cantidad; i++){
    int esValido = posicionValida(cuartos, vecinos->pos[i], cantidadDeCuartosActual);
    printf("Validando si el cuarto es valido: %d \n\n", esValido);
  }
  
}
