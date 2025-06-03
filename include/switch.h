//
// Created by alexis on 5/22/25.
//
#pragma once



#include <stdio.h>
#include "adresse.h"

// represente un switch d'un réseau
typedef struct Switch{
    AdresseMac adresseMac;     // adr mac unique du switch
    size_t nb_ports;
    int priorite;
    size_t *table_commutation;      //pour associer une adresse mac à un port du switch
} Switch;

Switch creer_switch(char* mac_str, size_t nb_port, int priorite);
void ajouter_commutation(Switch* sw, AdresseMac mac, size_t port);  // pour ajt une entrée dans la table
void afficher_switch(Switch *sw);
void afficher_table_commutation(Switch *sw);