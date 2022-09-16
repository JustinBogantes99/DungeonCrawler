
#include <stdio.h>
#include <stdlib.h>
#include <Structs.c>
/* creacion e inicializacion de la matrices en NULL */

long** intCreateMatrix(int n){
    long **arr;
    int i,j;
    arr = (long **)malloc(n * sizeof(Cuarto *));
    for (i=0; i<n; i++)
        arr[i] = (long *)malloc(n * sizeof(Cuarto));
    return arr;

}

long** intInitializeMatrix(long **arr,int n){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++)
            arr[i][j] = NULL;
    }

    return arr;

}