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

int cantidadDeCuartosActual;
int dificultad;
personaje monstruos[15];
objeto objetos[30];
personaje heroe;
cuarto cuartos[30] = {0};
int generarMapa(int[][30], int);

// Inicio del programa
// params:
//  void
int main(void) {
  int mapa[30][30] = {{0}};
  
  //pthread_t guess;

  //assert (pthread_create (&guess, NULL, keyboard_listener, NULL) == 0);


  srand(time(NULL));


  int a = generarMapa(mapa,10);
  printf("Ya sali del generarMapa");
  /*void *result;
  pthread_join (guess, &result);

  if (result)
    printf ("You must have guessed 7.\n");*/

  return 0;
}


// Función para inicializar los valores de la matriz mapa a 1
// params:
//  int max puede ser 10, 20, 30 dependiendo de la dificultad
int generarMapa(int mapa[][30], int max) {
  cantidadDeCuartosActual = 0;
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
  int cantidadFactibles = 0;
  posicion posiblesPosiciones[4];

  
  //Por aqui empieza el for para generar los cuartos :)
  for(int iterador = 1; iterador < 10; iterador++){
    for(int i = 0; i < vecinos->cantidad; i++){
      if (posicionValida(cuartos, vecinos->pos[i], cantidadDeCuartosActual) == 1){
        posiblesPosiciones[cantidadFactibles] = vecinos->pos[i];
        cantidadFactibles++;
      }
    }

    int cuartoACrear = rand() % cantidadFactibles;
    cantidadFactibles = 0;
    cuarto nuevoCuarto;
    nuevoCuarto.pos = posiblesPosiciones[cuartoACrear];
    nuevoCuarto.tipoCuarto = 1;
    nuevoCuarto.hayMonstruo = 0;
    nuevoCuarto.cantidadDeCuartosVecinos = 0;
    cuartos[iterador] = nuevoCuarto;
    printf("Agregue el cuarto con X %d  Y %d \n", cuartos[iterador].pos.x, cuartos[iterador].pos.y);
    vecinos = todosLosVecinos(mapa, cuartos[iterador], max);
    
    printf("Terminando el for\n\n\n");
    
  }
  return 0;
  
}
