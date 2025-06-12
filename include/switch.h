//
// Created by alexis on 5/22/25.
//
#pragma once



#include <stdio.h>
#include "adresse.h"
//#include "reseau.h"
#include "graphe.h"

struct reseau;  
typedef struct reseau reseau;

#define MAX_ENTREES_COMMUTATION 128

typedef struct {
    AdresseMac mac;
    int priorite; // pour Root ID
    int port;  // pour connaître le port par lequel ce BPDU a été reçu
    int cout;     // coût total jusqu’à la racine
} BPDU;

typedef struct {
    AdresseMac mac;     // adr mac
    size_t port;    // port du switch sur laquelle la mac a été enregustré
    int valide;        // 0 si false, 1 si true, pour savoir si le port est occupé
} EntreeTableCommutation;

typedef enum {
    PORT_BLOQUE,
    PORT_ACTIF_RACINE,
    PORT_ACTIF_DESIGNE
} EtatPort;

typedef struct Switch_s {
    AdresseMac adresseMac;
    size_t nb_ports;
    int priorite;

    EntreeTableCommutation table_commutation[MAX_ENTREES_COMMUTATION];
    size_t nb_asso;   // nombre d'associations actuellement enregistrées

    BPDU meilleur_bpdu;     // meilleur BPDU reçu
    size_t port_racine;     // port par lequel on reçoit le BPDU vers la racine
    int est_racine;     // est-ce que ce switch est la racine ?
    EtatPort *etat_ports;         // tab de taille nb_ports
} Switch_s;



Switch_s creer_switch(char* mac_str, size_t nb_port, int priorite);
void ajouter_commutation(Switch_s* sw, AdresseMac mac, size_t port);  // pour ajt une entrée dans la table
void afficher_switch(Switch_s *sw);
void afficher_table_commutation(Switch_s *sw);


void afficher_stp(reseau* reseau);
int get_port_vers_equipement(const graphe *g, int from_idx, int to_idx);
void appliquer_stp(reseau *r);
int comparer_bpdu(BPDU *a, BPDU *b);