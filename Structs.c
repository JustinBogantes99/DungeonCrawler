#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Cofre{
    int vida;
    int dmg;
};

struct Heroe{
    int vida;
    int dmg;
    int pos[2];
};


struct Monstruo{
    int vida;
    int dmg;
    int pos[2];
};

struct Cuarto{
    int pos[2];
    bool heroe;
    bool monstruo;
    struct Cofre tesoro;
    bool trampa;
};