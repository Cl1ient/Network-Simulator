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


void init_reseau(reseau *r);

void ajouter_station(reseau *r, station s);

void ajouter_switch(reseau *r, Switch sw);

void ajouter_lien(reseau *r, sommet s1, sommet s2);



#endif //RESEAU_H
