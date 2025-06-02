#include "../include/switch.h"
#include <stdio.h>
#include <string.h>

void init_switch(Switch *sw, AdresseMac *mac, int nb_ports, int priorite) {
    sw->mac = *mac;
    sw->nb_ports = nb_ports;
    sw->priorite = priorite;
    
    // Initialiser la table de commutation
    sw->nb_entrees_table = 0;
    for (int i = 0; i < MAX_TABLE_COMMUTATION; i++) {
        sw->table_commutation[i].valide = false;
    }
    
    // Initialiser les états des ports
    for (int i = 0; i < MAX_PORTS; i++) {
        sw->etats_ports[i] = PORT_INCONNU;
    }
    
    // Initialiser STP
    init_stp_switch(sw);
}

void afficher_switch(Switch *sw) {
    printf("Switch - MAC: ");
    afficher_AdresseMac(&sw->mac);
    printf(", Ports: %d, Priorité: %d\n", sw->nb_ports, sw->priorite);
    
    printf("États des ports: ");
    for (int i = 0; i < sw->nb_ports; i++) {
        printf("P%d:%s ", i, etat_port_to_string(sw->etats_ports[i]));
    }
    printf("\n");
    
    if (sw->est_racine) {
        printf("Ce switch est la RACINE du spanning tree\n");
    } else {
        printf("Racine: ");
        afficher_AdresseMac(&sw->racine_id);
        printf(", Coût: %d, Port racine: %d\n", sw->cout_racine, sw->port_racine);
    }
}

void afficher_table_commutation(Switch *sw) {
    printf("Table de commutation du switch ");
    afficher_AdresseMac(&sw->mac);
    printf(":\n");
    
    if (sw->nb_entrees_table == 0) {
        printf("  (table vide)\n");
        return;
    }
    
    for (int i = 0; i < sw->nb_entrees_table; i++) {
        if (sw->table_commutation[i].valide) {
            printf("  Port %d: ", sw->table_commutation[i].port);
            afficher_AdresseMac(&sw->table_commutation[i].mac);
            printf("\n");
        }
    }
}

void apprendre_adresse(Switch *sw, AdresseMac *mac, int port) {
    // Vérifier si l'adresse existe déjà
    for (int i = 0; i < sw->nb_entrees_table; i++) {
        if (sw->table_commutation[i].valide && 
            comparer_AdresseMac(&sw->table_commutation[i].mac, mac)) {
            // Mettre à jour le port si nécessaire
            sw->table_commutation[i].port = port;
            return;
        }
    }
    
    // Ajouter une nouvelle entrée si la table n'est pas pleine
    if (sw->nb_entrees_table < MAX_TABLE_COMMUTATION) {
        sw->table_commutation[sw->nb_entrees_table].mac = *mac;
        sw->table_commutation[sw->nb_entrees_table].port = port;
        sw->table_commutation[sw->nb_entrees_table].valide = true;
        sw->nb_entrees_table++;
        
        printf("Switch ");
        afficher_AdresseMac(&sw->mac);
        printf(" apprend: ");
        afficher_AdresseMac(mac);
        printf(" sur port %d\n", port);
    }
}

int chercher_port(Switch *sw, AdresseMac *mac) {
    for (int i = 0; i < sw->nb_entrees_table; i++) {
        if (sw->table_commutation[i].valide && 
            comparer_AdresseMac(&sw->table_commutation[i].mac, mac)) {
            return sw->table_commutation[i].port;
        }
    }
    return -1; // Adresse inconnue
}

void vider_table_commutation(Switch *sw) {
    sw->nb_entrees_table = 0;
    for (int i = 0; i < MAX_TABLE_COMMUTATION; i++) {
        sw->table_commutation[i].valide = false;
    }
}

void switch_recevoir_trame(Switch *sw, trame_ethernet *trame, int port_entree) {
    printf("Switch ");
    afficher_AdresseMac(&sw->mac);
    printf(" reçoit trame sur port %d\n", port_entree);
    
    // Apprendre l'adresse source
    apprendre_adresse(sw, &trame->source, port_entree);
    
    // Chercher le port de destination
    int port_dest = chercher_port(sw, &trame->dest);
    
    if (port_dest != -1 && port_dest != port_entree) {
        // Unicast: envoyer vers le port spécifique
        if (sw->etats_ports[port_dest] == PORT_RACINE || sw->etats_ports[port_dest] == PORT_DESIGNE) {
            printf("  -> Envoi unicast vers port %d\n", port_dest);
        } else {
            printf("  -> Port %d bloqué, trame abandonnée\n", port_dest);
        }
    } else {
        // Broadcast: diffuser sur tous les ports actifs sauf celui d'entrée
        printf("  -> Diffusion sur tous les ports actifs\n");
        switch_diffuser_trame(sw, trame, port_entree);
    }
}

void switch_diffuser_trame(Switch *sw, trame_ethernet *trame, int port_exclus) {
    for (int i = 0; i < sw->nb_ports; i++) {
        if (i != port_exclus && 
            (sw->etats_ports[i] == PORT_RACINE || sw->etats_ports[i] == PORT_DESIGNE)) {
            printf("    Diffusion vers port %d\n", i);
        }
    }
}

void init_stp_switch(Switch *sw) {
    // Au démarrage, chaque switch se considère comme racine
    sw->racine_id = sw->mac;
    sw->cout_racine = 0;
    sw->port_racine = -1;
    sw->est_racine = true;
    
    // Tous les ports sont initialement dans un état inconnu
    for (int i = 0; i < sw->nb_ports; i++) {
        sw->etats_ports[i] = PORT_INCONNU;
    }
}

char* etat_port_to_string(etat_port etat) {
    switch (etat) {
        case PORT_INCONNU: return "INCONNU";
        case PORT_RACINE: return "ROOT";
        case PORT_DESIGNE: return "DESIGNE";
        case PORT_BLOQUE: return "BLOQUE";
        default: return "???";
    }
}