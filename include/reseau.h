#pragma once

#include "graphe.h"
#include "station.h"
#include "switch.h"
#include "trame.h"

#define MAX_EQUIPEMENTS 100

// Types d'équipements
typedef enum {
    EQUIPEMENT_STATION = 1,
    EQUIPEMENT_SWITCH = 2
} type_equipement;

typedef union {
    station st;
    Switch sw;
} union_equipement;

// équipement
typedef struct {
    type_equipement type;
    union_equipement equipement;
} equipement;

// réseau
typedef struct {
    graphe graphe;  // Graphe représentant le réseau
    equipement equipements[MAX_EQUIPEMENTS];
    int nb_equipements;
    int nb_switches;  // Nombre de switch
    // on s'en fout du nb de stations, parceque c'est tout ce qui reste
} reseau;

void init_reseau(reseau *r);
void deinit_reseau(reseau *r);
void afficher_reseau(reseau *r);

// ajt des équipements
int ajouter_station(reseau *r, AdresseMac *mac, AdresseIP *ip);
int ajouter_switch(reseau *r, AdresseMac *mac, int nb_ports, int priorite);
void ajouter_lien(reseau *r, int equipement1, int equipement2, int cout);

// simulations
void simuler_envoi_trame(reseau *r, int station_src, AdresseMac *dest, uint16_t type, uint8_t *data, size_t taille);
void propager_trame(reseau *r, trame_ethernet *trame, int equipement_src, int port_src);

