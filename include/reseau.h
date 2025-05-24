//
// Created by alexis on 5/22/25.


#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"
#include "switch.h"
#include "station.h"

typedef struct reseau{
    graphe graphe; // structure du reseau sous forme de grapghe
    station *stations;   // liste des stations du reseau
    size_t nb_stations;
    Switch *switchs;
    size_t nb_switch;
} reseau;

reseau creer_reseau();  // creation d'un reseau vide
bool ajouter_station(reseau *reseau, station nv);
bool ajouter_switch(reseau *reseau, Switch nv);
bool connecter_equipement(reseau *reseau, size_t id1, size_t id2, int poids);   // connecter deux equipement (ajouter un arete)
void afficher_reseau(reseau *reseau);
