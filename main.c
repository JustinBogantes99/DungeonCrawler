#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>

#include "mapa.h"
#include "tiposDatos.h"


GtkWidget	  *window;
GtkWidget	  *fixed1;
GtkGrid	    *grid1;
GtkWidget   *imageHeroe;
GtkWidget   *imageMonstruo;
GtkWidget   *imagePiso;
GtkWidget   *imageCofre;
GtkWidget   *imageCuarto;
GtkWidget	  *buttonE;
GtkWidget	  *buttonM;
GtkWidget	  *buttonH;
GtkWidget   *buttonInicio;
GtkWidget   *box1;
GtkWidget   *imagenInicio;
GtkBuilder	*builder; 
GList *children, *iter;



int mapa[30][30] = {{0}};//matrix donde se van a crear los cuartos
int cantidadDeCuartosActual;//cantidad de cuartos
int dificultad;//dificultad que tendra el juego puede ser 10,20,30
personaje monstruos[15];//arreglo con el maximo de enemigos totales
objeto objetos[30];//arreglo con los objetos tipo trampa o tesoro
personaje heroe;//objetivo tipo personaje que tendra el roll de usuario
cuarto cuartos[30] = {0};//arreglo donde se almacenan todos los objeto tipo 
void generarMapa(int[][30], int);//funcion para generar el mapa
int max;
pthread_mutex_t mutex[30];//arreglo de mutex
pthread_mutex_t mutexMonstruos[15];//arreglo de mutex para monstruos
pthread_mutex_t mutexHeroe;//hilo mutex para el heroe
pthread_t HilosMonstruo[15];//arreglo de hilos para los monstruos

//funcion que permite recibir del teclado hacia donde se va a mover el heroe
void *keyboard_listener(void *args);
//--------------------------------------------------------------------------------------
int encontrarIndexCuarto(posicion pos) {
  int lengCuartos = sizeof(cuartos) / sizeof(cuartos[0]);
  for (int i = 0; i < lengCuartos; i++) {
    if (cuartos[i].pos.x == pos.x && cuartos[i].pos.y == pos.y) {
      return i;
    }
  }
  return -1;
}
//--------------------------------------------------------------------------------------
int encontrarIndexMonstruo(posicion pos) {
  int lengMonstruos = sizeof(monstruos) / sizeof(monstruos[0]);
  for (int i = 0; i < lengMonstruos; i++) {
    if (monstruos[i].pos.x == pos.x && monstruos[i].pos.y == pos.y) {
      return i;
    }
  }
  return -1;
}
//-------------------------------------------------------------------------------------- 
// Esta funcion permite mover el monstruo de un cuarto a otro
//-------------------------------------------------------------------------------------- 

void mover(int *monstruo_indx) {
  //printf("entre a mover \n");
  int monstruo = monstruo_indx;
  monstruos[monstruo].estado=1;
  int flag = 0; // bandera para indicar que se movio de cuarto el monstruo
  int random;
  int indexCuartoViejo;
  int indexCuartoNuevo;
  posicion vecinos[3];
  posicion nada;
  nada.x = -1;
  nada.y = -1;
  // inicializo el array con nada para comparar posiciones vacias con -1
  vecinos[0] = nada;
  vecinos[1] = nada;
  vecinos[2] = nada;
  int index = 0;
  // sacar los vecinos y agregarlos a array vecinos
  for (int i = 0; i < max; i++) {
    if ((cuartos[i].pos.x + 1 == (monstruos[monstruo].pos.x) ||
         cuartos[i].pos.x - 1 == (monstruos[monstruo].pos.x)) &&
        cuartos[i].pos.y == monstruos[monstruo].pos.y) {

      vecinos[index] = cuartos[i].pos;
      index++;
    }
    if ((cuartos[i].pos.y - 1 == (monstruos[monstruo].pos.y) ||
         cuartos[i].pos.y + 1 == (monstruos[monstruo].pos.y)) &&
        cuartos[i].pos.x == monstruos[monstruo].pos.x) {

      vecinos[index] = cuartos[i].pos;
      index++;
    }
  }

  // Ciclo para mover al monstruo de cuarto
  while (flag == 0) {
   
    random = rand() % 3;
    //printf("Cuarto elegido para mover al monstruo x: %i y: %i \n", vecinos[random].x,vecinos[random].y);
    if (vecinos[random].x != -1) { // valida que sea una direccion valida
      //printf("if 1 \n");
      indexCuartoNuevo = encontrarIndexCuarto(
          vecinos[random]); // busca el index en el arreglo del nuevo cuarto

      if (cuartos[indexCuartoNuevo].hayMonstruo == 0 &&
          cuartos[indexCuartoNuevo].tipoCuarto ==
              1) { // revisa si no hay     monstruos en el cuarto, entra si no
                   // hay. tambien revisa el tipo de cuarto

        indexCuartoViejo = encontrarIndexCuarto(
            monstruos[monstruo].pos); // busca index del cuarto viejo
        pthread_mutex_lock(&mutex[indexCuartoViejo]);

        cuartos[indexCuartoViejo].hayMonstruo =
            0; // indica que ya no hay un monstruo en el cuarto en el que estaba
               // el monstruo
         pthread_mutex_unlock(&mutex[indexCuartoViejo]);
        
        monstruos[monstruo].pos =
            vecinos[random]; // cambia la posicion del monstruo

        pthread_mutex_lock(&mutex[indexCuartoNuevo]);
        cuartos[indexCuartoNuevo].hayMonstruo =
            1; // indica que hay un monstruo en el nuevo cuarto
        
        pthread_mutex_unlock(&mutex[indexCuartoNuevo]);

        flag = 1; // indica que ya se movio de cuarto el monstruo

        monstruos[monstruo].estado=2;
        
         //printf("Monstruo %i en cuarto x: %i y: %i \n", monstruo,
         //monstruos[monstruo].pos.x, monstruos[monstruo].pos.y);
      }
    }
  }
}
//-------------------------------------------------------------------------------------- 
//-------------------------------------------------------------------------------------- 

void monstruo(void *monstruo_indx) {

  int monstruo = *(int *)monstruo_indx;
  int random;
  int randomSleep;

  //printf("Vida del monstruo %i: %i\n", monstruo, monstruos[monstruo].vida);
  while (monstruos[monstruo].vida > 0) {
    random = rand() % 2;
     randomSleep = rand() % 4;
    if(heroe.vida<=0){
      pthread_exit(0);
    }

    if (monstruos[monstruo].pos.x == heroe.pos.x &&
      monstruos[monstruo].pos.y == heroe.pos.y) {
      pthread_mutex_lock(&mutexHeroe);
      heroe.vida--;
      printf("\n Putazo del monstruo %d\n", monstruo);
      pthread_mutex_unlock(&mutexHeroe);
      
      sleep(randomSleep);
    }

    if(random==0){
      if (monstruos[monstruo].pos.x == heroe.pos.x &&
        monstruos[monstruo].pos.y == heroe.pos.y) {
      pthread_mutex_lock(&mutexHeroe);
      heroe.vida--;
      printf("\n Putazo del monstruo %d\n", monstruo);
      pthread_mutex_unlock(&mutexHeroe);
      
      sleep(randomSleep);
    }
    }

     //printf("antes de mover \n");
    if(random==1){
     mover(monstruo);
      sleep(randomSleep);
    }


  }


    
  
  monstruos[monstruo].estado=0;
  printf("--------memori \n");
  int cuartodelmonstruo= encontrarIndexCuarto(monstruos[monstruo].pos);
  cuartos[cuartodelmonstruo].hayMonstruo=0;
}
//-------------------------------------------------------------------------------------- 
/*Esta Funcion se encarga de ubicar los monstruos en la cantidad de cuarto 
toma como parametros la cantidad de cuartos ingresados como maximo */
//-------------------------------------------------------------------------------------- 

void ubicarPersonajes(int max, int monstruosp) {
  // printf("entre a ubicar Personajes \n");
  posicion pos;
  int i = 0;
  while (i < monstruosp) {
    // printf("%i \n", i);
    int randomRoom = rand() % max;
    // printf("--%i \n", randomRoom);
    if (cuartos[randomRoom].tipoCuarto == 1 &&
        cuartos[randomRoom].hayMonstruo == 0) {
      // pos = cuartos[randomRoom].pos;
      cuartos[randomRoom].hayMonstruo = 1;
      monstruos[i].pos = cuartos[randomRoom].pos;
      ;
      // printf("Monstruo asignado %i a  x: %i  y: %i \n", i,
      // monstruos[i].pos.x, monstruos[i].pos.y);
      i++;
    }
  }
}
//-------------------------------------------------------------------------------------- 
/*Esta Funcion se encarga de ubicar los tesoros y trampas los cuartos
toma como parametros la cantidad de cuartos ingresados como maximo , posiciona cada cuarto dependiendo de la probabilidad cada cuarto podra estar 40% con probabilidad de tener trampa, 30% de probabilidad de ser tesoro y el resto de estar vacio */
//-------------------------------------------------------------------------------------- 

void ubicarTesorosXTrampas(int max) {
  int i = 0;
  posicion pos;
  objeto item;
  while (i < max - 1) {

    int randomRoom = rand() % 10;
    int randomPower = rand() % 2;
    printf("\n\n\nPARA EL CUARTO %d\n", i);
    printf("Intentando crear un objeto en x: %d  y: %d \n", cuartos[i].pos.x,
           cuartos[i].pos.y);
    printf("Para el cuarto de tipo: %d\n\n", cuartos[i].tipoCuarto);
    if (cuartos[i].tipoCuarto != 2) {
      if (randomRoom == 0 || randomRoom == 1 || randomRoom == 2 ||
          randomRoom == 3) {
        item.tipoObjeto = 0;
        item.pos.x = cuartos[i].pos.x;
        item.pos.y = cuartos[i].pos.y;
        item.vida = 0;
        item.ataque = 1;
      } else if (randomRoom == 4 || randomRoom == 5 || randomRoom == 6) {
        item.tipoObjeto = 1;
        item.pos.x = cuartos[i].pos.x;
        item.pos.y = cuartos[i].pos.y;
        if (randomPower == 0) {
          item.vida = 1;
          item.ataque = 0;
        } else if (randomPower == 1) {
          item.vida = 0;
          item.ataque = 1;
        }
      } else {
        item.tipoObjeto = -1;
        item.pos.x = -1;
        item.pos.y = -1;
        item.vida = -1;
        item.ataque = -1;
      }
      item.activo = 1;
      objetos[i] = item;
    } else {
      item.tipoObjeto = -1;
      item.pos.x = -1;
      item.pos.y = -1;
      item.vida = -1;
      item.ataque = -1;
      objetos[i] = item;
    }
    i++;
  }
}
//--------------------------------------------------------------------------------------
void verTesoroXTrampa(int max) {
  for (int i = 0; i < max-1; i++) {
    printf("\n\n\nPARA EL OBJETO %d\n", i);
    printf("En el cuarto x: %d y: %d \n", objetos[i].pos.x, objetos[i].pos.y);
    printf("Se encuentra el objeto de tipo: %d\n\n", objetos[i].tipoObjeto);
  }
}
//-------------------------------------------------------------------------------------- 
// Función para inicializar los valores de la matriz mapa a 1
// params: matriz 30x30 y el valor de cuartos seleccionados
//  int max puede ser 10, 20, 30 dependiendo de la dificultad
//-------------------------------------------------------------------------------------- 

void generarMapa(int mapa[][30], int max) {
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
  printf("Primer Cuarto: X: %d  Y: %d \n\n", cuartos[0].pos.x,
         cuartos[0].pos.y);

  returnVecinos *vecinos = todosLosVecinos(mapa, cuartos[0], max);
  int cantidadFactibles = 0;
  posicion posiblesPosiciones[4];

  // Por aqui empieza el for para generar los cuartos a partir del primer cuarto
  for (int iterador = 1; iterador < max; iterador++) {
    printf("%d \n", iterador);
    printf("%d \n", max);
    printf("Cantidad de vecinos %d \n", vecinos->cantidad);
    for (int i = 0; i < vecinos->cantidad; i++) {
      printf("Antes de posicion valida\n");
      if (posicionValida(cuartos, vecinos->pos[i], cantidadDeCuartosActual) ==
          1) {
        printf("Entré a posición valida\n\n");
        posiblesPosiciones[cantidadFactibles] = vecinos->pos[i];
        cantidadFactibles++;
      }
      printf("Fuera de posicion valida\n");
    }
    printf("\nEsta es la cantidad de factibles: %d\n", cantidadFactibles);
    int cuartoACrear = rand() % cantidadFactibles;
    cantidadFactibles = 0;
    cuarto nuevoCuarto;
    nuevoCuarto.pos = posiblesPosiciones[cuartoACrear];
    nuevoCuarto.tipoCuarto = 1;
    nuevoCuarto.hayMonstruo = 0;
    nuevoCuarto.cantidadDeCuartosVecinos = 0;
    cuartos[iterador] = nuevoCuarto;
    mapa[nuevoCuarto.pos.x][nuevoCuarto.pos.y] = 1;
    printf("Agregue el cuarto con X %d  Y %d  TIPO: %d \n", cuartos[iterador].pos.x,
           cuartos[iterador].pos.y, cuartos[iterador].tipoCuarto);
    vecinos = todosLosVecinos(mapa, cuartos[iterador], max);
    int tempIterador;
    tempIterador = iterador;
    while(vecinos->cantidad == 0){
      tempIterador--;
      vecinos = todosLosVecinos(mapa, cuartos[tempIterador], max);
    }
  }
}
//-------------------------------------------------------------------------------------- 
/*
Funcion que permite recibir de tecla un caracter , ya sea para moverse por el mapa, atacar a un enemigo y 
*/
//-------------------------------------------------------------------------------------- 

void *keyboard_listener(void *args) {
  int c;
  posicion posicion_nueva;
  int cuarto;
  int monstruo;
  system("/bin/stty raw");
  while ((c = getchar()) != 'q') {
    if(heroe.vida<=0){
      printf("El heroe ha muerto :c");
      
      pthread_exit(0);
    }

    if (c == 'w') {
      posicion_nueva.x = heroe.pos.x - 1;
      posicion_nueva.y = heroe.pos.y;
      cuarto = encontrarIndexCuarto(posicion_nueva);
      if (cuarto != -1) {
        if (cuartos[cuarto].hayMonstruo == 0) {
           pthread_mutex_lock(&mutexHeroe);
          heroe.pos.x--;
          printf("Posicion del Heroe x: %i y:%i \n", heroe.pos.x, heroe.pos.y);
          pthread_mutex_unlock(&mutexHeroe);
        }
      }
    }

    if (c == 's') {
      posicion_nueva.x = heroe.pos.x + 1;
      posicion_nueva.y = heroe.pos.y;
      cuarto = encontrarIndexCuarto(posicion_nueva);
      if (cuarto != -1) {
        if (cuartos[cuarto].hayMonstruo == 0) {
          pthread_mutex_lock(&mutexHeroe);
          heroe.pos.x++;
          printf("Posicion del Heroe x: %i y:%i \n", heroe.pos.x, heroe.pos.y);
          pthread_mutex_unlock(&mutexHeroe);
        }
      }
    }

    if (c == 'd') {
      posicion_nueva.x = heroe.pos.x;
      posicion_nueva.y = heroe.pos.y + 1;
      cuarto = encontrarIndexCuarto(posicion_nueva);
      if (cuarto != -1) {
        if (cuartos[cuarto].hayMonstruo == 0) {
          pthread_mutex_lock(&mutexHeroe);
          heroe.pos.y++;
          printf("Posicion del Heroe x: %i y:%i \n", heroe.pos.x, heroe.pos.y);
          pthread_mutex_unlock(&mutexHeroe);
        }
      }
    }

    if (c == 'a') {
      posicion_nueva.x = heroe.pos.x;
      posicion_nueva.y = heroe.pos.y - 1;
      cuarto = encontrarIndexCuarto(posicion_nueva);
      if (cuarto != -1) {
        if (cuartos[cuarto].hayMonstruo == 0) {
          pthread_mutex_lock(&mutexHeroe);
          heroe.pos.y--;
          printf("\n\n\nPosicion del Heroe x: %i y:%i \n\n\n", heroe.pos.x, heroe.pos.y);
          pthread_mutex_unlock(&mutexHeroe);
        }
      }
    }

    if (c == 'e') {
      for(int i=0; i<max;i++){
        if(heroe.pos.x==objetos[i].pos.x && heroe.pos.y==objetos[i].pos.y){
          if(objetos[i].tipoObjeto==1 && objetos[i].activo==1){
            if(objetos[i].vida==1){
              pthread_mutex_lock(&mutexHeroe);
              heroe.vida++;
              printf("\n\n Ha abierto un tesoro. Ahora tiene +1 Vida\n\n");
              pthread_mutex_unlock(&mutexHeroe);
            }
            else{
              heroe.ataque++;
              printf("\n\n Ha abierto un tesoro. Ahora tiene +1 Ataque\n\n");
            }
            objetos[i].activo=0;
          }
        }
      }
    }


    if(c == ' '){
      monstruo = encontrarIndexMonstruo(heroe.pos);
      if(monstruo!=-1){
        if(monstruos[monstruo].estado==2){
          
          pthread_mutex_lock(&mutexMonstruos[monstruo]);
          monstruos[monstruo].vida-=heroe.ataque;
          pthread_mutex_unlock(&mutexMonstruos[monstruo]);
        }
      }
    }
    sleep(0.1);


    for(int i=0; i<max;i++){
        if(heroe.pos.x==objetos[i].pos.x && heroe.pos.y==objetos[i].pos.y){
          if(objetos[i].tipoObjeto==0 && objetos[i].activo==1){
            
            heroe.vida--;
            printf("\n\n Ha caído en la trampa de que ella te ama ;c Que zad\n\n");
            
            objetos[i].activo=0;
          }
        }
      }

    printf("\n\n El heroe tiene las siguientes estadisticas: VIDA: %d  ATAQUE: %d \n", heroe.vida, heroe.ataque);
    printf("Y se encuentra en la posición actual de: X: %d  Y: %d", heroe.pos.x, heroe.pos.y);
    cuarto = encontrarIndexCuarto(heroe.pos);
    if(cuartos[cuarto].tipoCuarto==2){
      pthread_exit((void *)false);
      printf("CUARTO FINAL");
    }
  }
  system("/bin/stty cooked");
  pthread_exit((void *)false);
}


//-------------------------------------------------------------------------------------- 

/*
Esta funcion me permite refrescar la interfaz
*/
gboolean refrescar(){

  
  children = gtk_container_get_children(GTK_CONTAINER(grid1));
  
  for(iter = children; iter != NULL; iter = g_list_next(iter)){
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  }

  g_list_free(children);



  

  for(int i = 0; i < max; i++){  //Esta si debe ser max ya que es la matriz NxN
    for(int j = 0; j < max; j++){

    imagePiso = gtk_image_new_from_file("images/init.png");
    gtk_grid_attach(GTK_GRID(grid1), imagePiso, i, j, 1, 1);

    }
  }

//-------------------------------------------------------------------------------------- 

  for(int i = 0; i < max; i++){   

    if(cuartos[i].tipoCuarto == 0){

      imageCuarto = gtk_image_new_from_file("images/salida.png");
      gtk_grid_attach(GTK_GRID(grid1), imageCuarto, cuartos[i].pos.y, cuartos[i].pos.x, 1, 1);
    }else if(cuartos[i].tipoCuarto == 2){

      imageCuarto = gtk_image_new_from_file("images/final.png");
      gtk_grid_attach(GTK_GRID(grid1), imageCuarto, cuartos[i].pos.y, cuartos[i].pos.x, 1, 1);

    }else{
      imageCuarto = gtk_image_new_from_file("images/cuarto.png");
      gtk_grid_attach(GTK_GRID(grid1), imageCuarto, cuartos[i].pos.y, cuartos[i].pos.x, 1, 1);
    }

  }
  

//-------------------------------------------------------------------------------------- 


  for(int i = 0; i < max; i++){

    if(objetos[i].tipoObjeto == 1 && objetos[i].activo == 1 && objetos[i].pos.y >= 0 && objetos[i].pos.x >= 0 ){

    imageCofre = gtk_image_new_from_file("images/cofre.png");
    gtk_grid_attach(GTK_GRID(grid1), imageCofre, objetos[i].pos.y, objetos[i].pos.x, 1, 1);

    }
  }


//-------------------------------------------------------------------------------------- 

  imageHeroe = gtk_image_new_from_file("images/heroe.png");
  gtk_grid_attach(GTK_GRID(grid1), imageHeroe, heroe.pos.y, heroe.pos.x, 1, 1);
  

//-------------------------------------------------------------------------------------- 

  for(int i = 0; i < max/2; i++){
    if(monstruos[i].estado != 0){
      
      imageMonstruo = gtk_image_new_from_file("images/monstruo.png");
      gtk_grid_attach(GTK_GRID(grid1), imageMonstruo, monstruos[i].pos.y, monstruos[i].pos.x, 1, 1);

    }
  }

  gtk_widget_show(window);
  gtk_widget_show_all(grid1);
  gtk_widget_queue_draw(grid1);



  return TRUE;
}



void *llamada_ventana(){



  gtk_init(NULL, NULL); // init Gtk
 
  builder = gtk_builder_new_from_file ("prueba.glade"); //Abre el XML que genera glade
 
  window = GTK_WIDGET(gtk_builder_get_object(builder, "ventana1")); //El nombre de la estructura aka ID

  fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1")); // Recordar que se usa el nombre del ID en glade
  grid1 = GTK_GRID(gtk_builder_get_object(builder, "matrix"));

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); 

  gtk_builder_connect_signals(builder, NULL);


  g_timeout_add(100,(GSourceFunc)refrescar, NULL);
  gtk_main();

}




// Inicio del programa
// params:
//  void
int main(void) {

  int opcion;
  
  printf("Por favor ingrese la opcion de la dificultad que desea jugar.\n");
  printf("1. Dificultad Facil. \n2. Dificultad Media. \n3. Dificultad Dificil.\n\n\n");

  scanf("%d", &opcion);  
  printf("%d", opcion);

  switch(opcion){
  
  case 1:
  	
  	max = 10;
  	break;
  	
  case 2:
  	
  	max = 20;
  	break;
  	
  case 3:
  	
  	max = 30;
  	break;
  	
  
  }

  srand(time(0));
  generarMapa(mapa, max);
  int cuartoInicio = max - 1;
  int cuartoFinal = rand() % max - 1;
  cuartos[cuartoInicio].tipoCuarto = 0;
  cuartos[cuartoFinal].tipoCuarto = 2;
  ubicarTesorosXTrampas(max);
  verTesoroXTrampa(max);
  int cantidadMonsters = max / 2;
  int arregloAntibugs[cantidadMonsters]; // arreglo para guardar los indices de
                                         // monstruos, si se usa el i del for no
                                         // funciona bien
  
  // Instancear heroe
  heroe.ataque = 1;
  heroe.vida = 4;
  heroe.tipo = 0;
  heroe.pos = cuartos[cuartoInicio].pos;

  pthread_t HilosMonstruo[cantidadMonsters];

  personaje monster;
  for (int x = 0; x < cantidadMonsters; x++) {
    // printf("asignando mosntruo %i \n", x);
    monster.tipo = 1;
    monster.vida = 3;
    monster.ataque = 1;
    monstruos[x] = monster;
  }

  ubicarPersonajes(max, cantidadMonsters);
  
  //crear mutex de los cuartos
  for(int m=0; m<max;m++){
    pthread_mutex_init(&mutex[m], NULL);
  }

  // printf("Termine de asignar monstruos \n");
  // creacion de mutex e hilos de los monstruos
  for (int i = 0; i < cantidadMonsters; i++) {
    pthread_mutex_init(&mutexMonstruos[i], NULL);
    arregloAntibugs[i] = i;
    // printf("Creando hilo del monstruo %i \n", i);
   pthread_create(&HilosMonstruo[i], NULL, (void *)&monstruo,
                   (void *)&arregloAntibugs[i]);
  }

  pthread_t GUI;
  pthread_create(&GUI, NULL, (void *)&llamada_ventana,NULL);

  printf("Cuarto final en x: %i y:%i \n",cuartos[cuartoFinal].pos.x, cuartos[cuartoFinal].pos.y);
  pthread_t hiloHeroe;
  pthread_mutex_init(&mutexHeroe, NULL);
  assert(pthread_create(&hiloHeroe, NULL, keyboard_listener, NULL) == 0);


  // join de los hilos de los monstruos
  //for (int i = 0; i < cantidadMonsters; i++) {
  //  pthread_join(HilosMonstruo[i], NULL);
  //}


  pthread_join(hiloHeroe, NULL);
  pthread_join(GUI, NULL);

  return 0;
}

