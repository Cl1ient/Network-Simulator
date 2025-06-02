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


void envoyer_trame_via_tous_les_switchs(reseau *r, size_t id_src, size_t id_dst, uint8_t *donnees, size_t longueur) {
    if (!r || id_src >= r->nb_stations || id_dst >= r->nb_stations) {
        printf("ID invalide pour les stations.\n");
        return;
    }

    TrameEthernet trame;

    // Remplir la trame
    memset(&trame, 0, sizeof(TrameEthernet));
    memset(trame.preambule, 0xAA, 7);  // Préambule
    trame.sfd = 0xAB;

    trame.adresse_source = r->stations[id_src].mac;
    trame.adresse_destination = r->stations[id_dst].mac;

    trame.type[0] = 0x08;
    trame.type[1] = 0x00;

    memcpy(trame.data, donnees, longueur);

    // FCS bidon (exemple)
    memset(trame.fcs, 0xDE, 4);

    // Passage par tous les switchs dans l’ordre de création
    printf("\n--- Début de l'envoi ---\n");
    printf("Station source [%zu] => Station destination [%zu]\n", id_src, id_dst);

    for (size_t i = 0; i < r->nb_switch; i++) {
        printf("→ Passage par Switch %zu : ", i);
        afficherMac(r->switchs[i].adresseMac);
        ajouter_commutation(&r->switchs[i], trame.adresse_source, 0);
        ajouter_commutation(&r->switchs[i], trame.adresse_destination, 1);
    }

    printf("\nTrame reçue par la station destination :\n");
    afficherTrameUser(&trame, longueur);
}

void renvoyer_tram() {

}