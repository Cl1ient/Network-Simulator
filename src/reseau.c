#include "../include/reseau.h"
#include "../include/graphe.h"
#include "../include/algos.h"
#include "../include/tram.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

reseau creer_reseau() {
    reseau r;
    r.nb_stations = 0;
    r.nb_switch = 0;
    r.stations = NULL;
    r.switchs = NULL;

    init_graphe(&r.graphe);

    return r;
}

// ajt d'une station dans le rres
bool ajouter_station(reseau *reseau, station nv) {
    reseau->stations = realloc(reseau->stations, (reseau->nb_stations + 1) * sizeof(station));
    reseau->stations[reseau->nb_stations] = nv;
    reseau->nb_stations++;
    return true;

}

// ajt switch
bool ajouter_switch(reseau *reseau, Switch nv) {
    reseau->switchs = realloc(reseau->switchs, (reseau->nb_switch + 1) * sizeof(Switch));
    reseau->switchs[reseau->nb_switch] = nv;
    reseau->nb_switch++;
    return true;
}

// Connexion de deux équipements (ajt d’une arete dans le graphe)
bool connecter_equipement(reseau *reseau, size_t id1, size_t id2, int poids) {
    if (!reseau) {
        return false;
    }

    arete nouvelle_arete = {id1, id2, poids};

    return ajouter_arete(&reseau->graphe, nouvelle_arete);
}



// Affichage du réseau
void afficher_reseau(reseau *reseau) {
    if (!reseau) {
        printf("Réseau invalide\n");
        return;
    }

    printf("Réseau\n");
    printf("Stations : %zu\n", reseau->nb_stations);
    for (size_t i = 0; i < reseau->nb_stations; i++) {
        afficher_station(&reseau->stations[i]);
    }

    printf("\nSwitchs : %zu\n", reseau->nb_switch);
    for (size_t i = 0; i < reseau->nb_switch; i++) {
        afficher_switch(&reseau->switchs[i]);
    }

    printf("\nStructure du graphe :\n");
    afficher(&reseau->graphe);
}


// Fonction récursive d'envoi
void envoyer_trame_recursif(reseau *r, size_t courant, size_t id_dst, TrameEthernet *trame, int *visites) {
    printf("Appel envoyer_trame_recursif sur %zu\n", courant); // Ajoute cette ligne

    
    if (visites[courant]) return;
    visites[courant] = 1;

    // Si on est sur la station de destination
    if (courant == id_dst) {
        printf("\nTrame reçue par la station destination [%zu] :\n", id_dst);
        afficherTrameUser(trame, 46); // ou longueur réelle
        return;
    }

    // Si on est sur un switch, mettre à jour la table de commutation
    if (courant >= r->nb_stations) {
        size_t id_switch = courant - r->nb_stations;
        printf("→ Passage par Switch %zu : ", id_switch);
        afficherMac(r->switchs[id_switch].adresseMac);
        ajouter_commutation(&r->switchs[id_switch], trame->adresse_source, 0);
        ajouter_commutation(&r->switchs[id_switch], trame->adresse_destination, 1);
    }

    // Parcourir les voisins dans le graphe
    for (size_t i = 0; i < r->graphe.nb_aretes; i++) {
        arete a = r->graphe.aretes[i];
        size_t voisin = SIZE_MAX;
        if (a.s1 == courant) voisin = a.s2;
        else if (a.s2 == courant) voisin = a.s1;
        else continue;

        printf("Appel récursif de %zu vers %zu\n", courant, voisin); // Ajoute cette ligne
        envoyer_trame_recursif(r, voisin, id_dst, trame, visites);
    }
}

void envoyer_trame_via_tous_les_switchs(reseau *r, size_t id_src, size_t id_dst, uint8_t *donnees, size_t longueur) {
    if (!r || id_src >= r->nb_stations || id_dst >= r->nb_stations) {
        printf("ID invalide pour les stations.\n");
        return;
    }

    TrameEthernet trame;
    memset(&trame, 0, sizeof(TrameEthernet));
    memset(trame.preambule, 0xAA, 7);
    trame.sfd = 0xAB;
    trame.adresse_source = r->stations[id_src].mac;
    trame.adresse_destination = r->stations[id_dst].mac;
    trame.type[0] = 0x08;
    trame.type[1] = 0x00;
    memcpy(trame.data, donnees, longueur);
    memset(trame.fcs, 0xDE, 4);

    printf("\n--- Début de l'envoi ---\n");
    printf("Station source [%zu] => Station destination [%zu]\n", id_src, id_dst);

    int *visites = calloc(r->nb_stations + r->nb_switch, sizeof(int));
    envoyer_trame_recursif(r, id_src, id_dst, &trame, visites);
    free(visites);
}

void renvoyer_tram() {

}