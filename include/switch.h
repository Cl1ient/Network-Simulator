//
// Created by alexis on 5/22/25.
//
#pragma once



#include <stdio.h>
#include "adresse.h"

#define MAX_ENTREES_COMMUTATION 128


typedef struct {
    AdresseMac mac;     // adr mac
    size_t port;    // port du switch sur laquelle la mac a été enregustré
    int valide;        // 0 si false, 1 si true, pour savoir si le port est occupé
} EntreeTableCommutation;

typedef struct Switch_s {
    AdresseMac adresseMac;
    size_t nb_ports;
    int priorite;
    EntreeTableCommutation table_commutation[MAX_ENTREES_COMMUTATION];
    size_t nb_asso;   // nombre d'associations actuellement enregistrées
} Switch_s;



Switch_s creer_switch(char* mac_str, size_t nb_port, int priorite);
void ajouter_commutation(Switch_s* sw, AdresseMac mac, size_t port);  // pour ajt une entrée dans la table
void afficher_switch(Switch_s *sw);
void afficher_table_commutation(Switch_s *sw);

