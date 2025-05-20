#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"

typedef struct reseau{
    graphe graphe; // struct graphe dans graphe.h
    size_t nbEquipements; // nombre de station/switch
    size_t nbLiens; // nombre d'arrete
} reseau;