//
// Created by acelya on 23/05/2025.
//

#include "stdio.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/adresse.h"
#include "../include/switch.h"
#include "../include/station.h"
#include "../include/reseau.h"

Switch_s creer_switch(char* mac_str, size_t nb_ports, int prio) {
    Switch_s sw;

    int mac_ok = string_to_mac(mac_str, &sw.adresseMac);   // on verifie si l'adresse est ok, et si c'est ok on le stock dans sw.mac
    if (!mac_ok) {
        printf("Erreur: mac invalide\n");
    }

    sw.nb_ports = nb_ports;
    sw.priorite = prio;
    sw.nb_asso = 0;

    // on initialise toutes les entrées comme invalides
    for (size_t i = 0; i < MAX_ENTREES_COMMUTATION; i++) {
        sw.table_commutation[i].valide = 0;
    }

    return sw;
}

// ajt d'une entrée dans la table
void ajouter_commutation (Switch_s* sw, AdresseMac mac, size_t port) {
    if (!sw) {
        return;
    }

    if (sw->nb_asso >= MAX_ENTREES_COMMUTATION) {
        printf("Table de commutation pleine\n");
        return;
    }

    // Si la mac y est déja, on ne la rajoute pas.
    for (int i = 0; i < sw->nb_asso; i++) {
        if (comparer_mac(sw->table_commutation[i].mac, mac) && sw->table_commutation[i].port == port) {
            // Déjà présent : ne rien faire
            return;
        }
    }

    sw->table_commutation[sw->nb_asso].mac = mac;
    sw->table_commutation[sw->nb_asso].port = port;
    sw->table_commutation[sw->nb_asso].valide = 1;
    sw->nb_asso++;

}

void afficher_switch(Switch_s* sw) {
    printf("Switch: \n");
    afficherMac(sw->adresseMac);
    printf("Nb de ports : %zu\n", sw->nb_ports);
    printf("Prio STP : %d\n", sw->priorite);
    afficher_table_commutation(sw);

}

void afficher_table_commutation(Switch_s *sw) {
    printf("Table de commutation du switch (MAC → port) :\n");
    for (size_t i = 0; i < sw->nb_ports; i++) {
        if (sw->table_commutation[i].valide) {
            char mac_str[18];
            mac_to_string(sw->table_commutation[i].mac, mac_str);
            printf("MAC %s → Port %zu\n", mac_str, sw->table_commutation[i].port);
        }
    }

}




