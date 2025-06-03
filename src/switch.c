//
// Created by acelya on 23/05/2025.
//

#include "stdio.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/adresse.h"
#include "../include/switch.h"

Switch creer_switch(char* mac_str, size_t nb_ports, int prio) {
    Switch sw;

    int mac_ok = string_to_mac(mac_str, &sw.adresseMac);   // on verifie si l'adresse est ok, et si c'est ok on le stock dans sw.mac
    if (!mac_ok) {
        printf("Erreur: mac invalide\n");
    }

    sw.nb_ports = nb_ports;
    sw.priorite = prio;

    // on alloue la table de commut (chaque port peut stocker une adresse)
    sw.table_commutation = (size_t*) malloc(nb_ports * sizeof(size_t));
    if (!sw.table_commutation) {
        printf("L'allocation mémoire pour la table de commu a échoué");
    }
    else {
        memset(sw.table_commutation, 0, nb_ports * sizeof(size_t)); // on initialise à 0
    }
    return sw;
}

// ajt d'une entrée dans la table
void ajouter_commutation (Switch* sw, AdresseMac mac, size_t port) {
    if (!sw || port >= sw->nb_ports) {
        printf ("port invalide");
        return;
    }

    // ajt du mac au port donnée
    sw->table_commutation[port] = *((size_t*) mac.octets);
}

void afficher_switch(Switch* sw) {
    printf("Switch: \n");
    afficherMac(sw->adresseMac);
    printf("Nb de ports : %zu\n", sw->nb_ports);
    printf("Prio stp: %d\n", sw->priorite);

    printf("Table de commu: \n");
    for (size_t i = 0; i < sw->nb_ports; i++) {
        if (sw->table_commutation[i] != 0) {
            AdresseMac mac;
            memcpy(mac.octets, &sw->table_commutation[i], sizeof(AdresseMac));      // on recupere l'adresse mac stockée
            char mac_str[18];
            mac_to_string(mac, mac_str); // convertir la MAC en chaîne
            printf("Port %zu → MAC %s\n", i, mac_str);
        }
    }
}

void afficher_table_commutation(Switch *sw) {
    printf("Table de commutation du switch (MAC → port) :\n");
    for (size_t i = 0; i < sw->nb_ports; i++) {
        if (sw->table_commutation[i] != 0) {
            AdresseMac mac;
            memcpy(mac.octets, &sw->table_commutation[i], sizeof(AdresseMac));
            char mac_str[18];
            mac_to_string(mac, mac_str);
            printf("Port %zu → MAC %s\n", i, mac_str);
        }
    }
}