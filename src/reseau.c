#include "../include/reseau.h"
#include "../include/graphe.h"
#include "../include/algos.h"
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
bool ajouter_station(reseau *reseau, char* mac_str, char* ip_str) {
    if (!reseau) {
        return false;
    }

    station *nouvelle_liste = realloc(reseau->stations, (reseau->nb_stations + 1) * sizeof(station));
    if (!nouvelle_liste) {
        return false;
    }

    reseau->stations = nouvelle_liste;
    reseau->stations[reseau->nb_stations] = creer_station(mac_str, ip_str);
    reseau->nb_stations++;

    return true;
}

// ajt switch
bool ajouter_switch(reseau *reseau, char* mac_str, size_t nb_ports, int priorite) {
    if (!reseau) {
        return false;
    }

    Switch *nouvelle_liste = realloc(reseau->switchs, (reseau->nb_switch + 1) * sizeof(Switch));
    if (!nouvelle_liste) {
        return false;
    }

    reseau->switchs = nouvelle_liste;
    reseau->switchs[reseau->nb_switch] = creer_switch(mac_str, nb_ports, priorite);
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

