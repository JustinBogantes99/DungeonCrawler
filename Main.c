
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#include "structs"
/* creacion e inicializacion de la matrices en NULL */
typedef struct Cofres{
    int vida;
    int dmg;
}Cofre;

typedef struct Heroes{
    int vida;
    int dmg;
    int pos[2];
}Heroe;


typedef struct Monstruos{
    int vida;
    int dmg;
    int pos[2];
}Monstruo;

typedef struct Cuartos{
    int pos[2];
    bool heroe;
    bool monstruo;
    bool start;
    bool end;
    struct Cofres tesoro;
    bool trampa;
    int id;
}Cuarto; 

struct Cuarto** intCreateMatrix(int n){
    Cuarto ***Qw;

    Qw =  malloc(n * sizeof(Cuarto **));
    for(int i=0; i<n; i++){
        Qw[i] =  malloc((n+1) * sizeof(Cuarto *));
        for(int j=0; j<=n;j++)
            Qw[i][j] = malloc((n+1) * sizeof(Cuarto));
    }


    /**
    struct Cuarto **arr;
    int i,j;
    Cuarto CuartoGeneral;
    arr = (struct Cuarto **)malloc(n * sizeof(struct Cuarto *));
    for (i=0; i<n; i++)
        arr[i] = (struct Cuarto*)malloc(n * sizeof( Cuarto));
    return arr;**/

}

Cuarto initRoom(int cont){
    Cuarto room ;
    room = (Cuarto) {
        .pos=NULL,
        .heroe=NULL,
        .monstruo=NULL,
        .tesoro=NULL,
        .trampa=NULL,
        .start=NULL,
        .end=NULL,
        .id=cont,
    };
    return room;
}

struct Cuarto** intInitializeMatrix(struct Cuarto **arr,int n){
    int i,j;
    Cuarto room;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++)
            room = initRoom(i);
            //room->id
    }
    return arr;

}
int main(){
    int n;
    printf("Inserte el n para crear las matrices: ");
    scanf("%d", &n);
    struct Cuarto **Mat;
    Mat = intCreateMatrix(n);
    Mat = intInitializeMatrix(Mat,n);

}