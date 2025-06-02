#include "../include/reseau.h"
#include "../include/graphe.h"
#include <stdio.h>
#include <string.h>

void init_reseau(reseau *r) {
    init_graphe(&r->graphe);
    r->nb_equipements = 0;
    r->nb_switches = 0;
}

void deinit_reseau(reseau *r) {
    deinit_graphe(&r->graphe);
    r->nb_equipements = 0;
    r->nb_switches = 0;
}

void afficher_reseau(reseau *r) {
    printf("=== RÉSEAU LOCAL ===\n");
    printf("Nombre d'équipements: %d\n", r->nb_equipements);
    printf("Nombre de liens: %zu\n", nb_aretes(&r->graphe));
    
    printf("\n--- ÉQUIPEMENTS ---\n");
    for (int i = 0; i < r->nb_equipements; i++) {
        printf("%d. ", i);
        if (r->equipements[i].type == EQUIPEMENT_STATION) {
            afficher_station(&r->equipements[i].equipement.st);
        } else {
            afficher_switch(&r->equipements[i].equipement.sw);
        }
    }
    
    printf("\n\n--- LIENS ---\n");
    for (size_t i = 0; i < r->graphe.nb_aretes; i++) {
        arete a = r->graphe.aretes[i];
        printf("Lien %zu -> %zu (coût: %d)\n", a.s1, a.s2, a.poids);
    }
    printf("==================\n");
}

int ajouter_station(reseau *r, AdresseMac *mac, AdresseIP *ip) {
    int index = r->nb_equipements;
    r->equipements[index].type = EQUIPEMENT_STATION;
    init_station(&r->equipements[index].equipement.st, mac, ip);
    
    ajouter_sommet(&r->graphe);
    r->nb_equipements++;
    
    printf("Station ajoutée à l'index %d\n", index);
    return index;
}

int ajouter_switch(reseau *r, AdresseMac *mac, int nb_ports, int priorite) {   
    int index = r->nb_equipements;
    r->equipements[index].type = EQUIPEMENT_SWITCH;
    init_switch(&r->equipements[index].equipement.sw, mac, nb_ports, priorite);
    
    ajouter_sommet(&r->graphe);
    r->nb_equipements++;
    r->nb_switches++;
    
    printf("Switch ajouté à l'index %d\n", index);
    return index;
}

void ajouter_lien(reseau *r, int equipement1, int equipement2, int cout) {    
    arete a = {equipement1, equipement2, cout};
    ajouter_arete(&r->graphe, a);
    printf("Lien ajouté: %d <-> %d (coût: %d)\n", equipement1, equipement2, cout);
}

void simuler_envoi_trame(reseau *r, int station_src, AdresseMac *dest, uint16_t type, uint8_t *data, size_t data_size) {    
    trame_ethernet trame;
    station *st = &r->equipements[station_src].equipement.st;
    
    station_envoyer_trame(st, dest, type, data, data_size, &trame);
    
    printf("\n=== PROPAGATION DE LA TRAME ===\n");
    propager_trame(r, &trame, station_src, -1);
    printf("===============================\n");
}

void propager_trame(reseau *r, trame_ethernet *trame, int equipement_src, int port_src) {
    // Trouver tous les équipements connectés à equipement_src
    sommet adjacents[MAX_EQUIPEMENTS];
    size_t nb_adjacents = sommets_adjacents(&r->graphe, equipement_src, adjacents);
    
    for (size_t i = 0; i < nb_adjacents; i++) {
        int equipement_dest = adjacents[i];
        
        if (r->equipements[equipement_dest].type == EQUIPEMENT_STATION) {
            // Si c'est une station, elle reçoit la trame
            station *st = &r->equipements[equipement_dest].equipement.st;
            
            // Vérifier si la trame lui est destinée
            if (comparer_AdresseMac(&trame->dest, &st->mac)) {
                printf("Trame arrivée à destination: ");
                station_recevoir_trame(st, trame);
            }
            
        } else if (r->equipements[equipement_dest].type == EQUIPEMENT_SWITCH) {
            // Si c'est un switch, il traite la trame
            Switch *sw = &r->equipements[equipement_dest].equipement.sw;
            
            // Trouver le numéro de port (simplifié)
            int port_entree = 0;
            switch_recevoir_trame(sw, trame, port_entree);
            
            // Le switch peut retransmettre (logique simplifiée)
            // Dans une vraie implémentation, il faudrait gérer la boucle de propagation
        }
    }
}

void reseau_initialiser_stp(reseau *r) {
    printf("=== INITIALISATION STP ===\n");
    
    // Initialiser STP sur tous les switches
    for (int i = 0; i < r->nb_switches; i++) {
        if (r->equipements[i].type == EQUIPEMENT_SWITCH) {
            init_stp_switch(&r->equipements[i].equipement.sw);
            printf("STP initialisé sur switch %d\n", i);
        }
    }
    
    printf("=========================\n");
}

