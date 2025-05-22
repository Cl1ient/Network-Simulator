//
// Created by alexis on 5/22/25.
//

#ifndef RESEAU_H
#define RESEAU_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"

typedef struct reseau{
    graphe graphe; // struct graphe dans graphe.h
    size_t nbEquipements; // nombre de station/switch
    size_t nbLiens; // nombre d'arrete
} reseau;
#endif //RESEAU_H
