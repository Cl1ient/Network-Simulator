//
// Created by alexis on 5/22/25.


#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"
#include "switch.h"
#include "station.h"
#include "tram.h"

#define MAX_EQUIPEMENTS 100


typedef enum {
    EQUIPEMENT_STATION = 1,
    EQUIPEMENT_SWITCH = 2
} type_equipement;

typedef union union_equipement {
    station st;
    Switch_s sw;
} union_equipement;

typedef struct {
    type_equipement type;
    union_equipement equipement;
} equipement;


typedef struct reseau{
    graphe graphe; // structure du reseau sous forme de grapghe
    equipement equipements[MAX_EQUIPEMENTS];
    int nb_equipements;
    int nb_switches;  // Nombre de switch
    // on s'en fout du nb de stations, parceque c'est tout ce qui reste
} reseau;



reseau creer_reseau();  // creation d'un reseau vide
bool ajouter_station(reseau *reseau, station nv);
bool ajouter_switch(reseau *reseau, Switch_s nv);
bool connecter_equipement(reseau *reseau, size_t id1, size_t id2, int poids);   // connecter deux equipement (ajouter un arete)
void afficher_reseau(reseau *reseau);
void envoyer_trame_via_tous_les_switchs(reseau *r, size_t id_src, size_t id_dst, uint8_t *donnees, size_t longueur);




size_t port_entre_de_precedent(reseau *r, size_t id_switch, size_t id_voisin);
size_t chercher_port_mac(Switch_s *sw, AdresseMac mac);
size_t voisin_sur_port(reseau *r, size_t id_switch, size_t port);
int envoyer_trame_rec(reseau *r, size_t id_actuel, TrameEthernet *trame, size_t id_precedent);
void envoyer_trame(reseau *r, size_t id_station_source, TrameEthernet *trame);