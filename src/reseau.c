#include "../include/reseau.h"
#include "../include/graphe.h"
#include "../include/algos.h"
#include "../include/tram.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

reseau creer_reseau() {
    reseau r;
    r.nb_equipements = 0;
    r.nb_equipements = 0;


    init_graphe(&r.graphe);

    return r;
}

// Connexion de deux équipements (ajt d’une arete dans le graphe)
bool connecter_equipement(reseau *reseau, size_t id1, size_t id2, int poids) {
    if (!reseau) {
        return false;
    }

    arete nouvelle_arete = {id1, id2, poids};

    return ajouter_arete(&reseau->graphe, nouvelle_arete);
}



void afficher_reseau(reseau *reseau) {
    if (!reseau) {
        printf("Réseau invalide\n");
        return;
    }
    printf("Réseau\n");
    printf("Équipements : %d\n", reseau->nb_equipements);
    for (int i = 0; i < reseau->nb_equipements; i++) {
        if (reseau->equipements[i].type == EQUIPEMENT_STATION)
            afficher_station(&reseau->equipements[i].equipement.st);
        else if (reseau->equipements[i].type == EQUIPEMENT_SWITCH)
            afficher_switch(&reseau->equipements[i].equipement.sw);
    }
    printf("\nStructure du graphe :\n");
    afficher(&reseau->graphe);
}


// Fonction récursive d'envoi
void envoyer_trame_recursif(reseau *r, size_t courant, size_t id_dst, TrameEthernet *trame, int *visites) {
    printf("Appel envoyer_trame_recursif sur %zu\n", courant);

    if (visites[courant]) return;
    visites[courant] = 1;

    // Si on est sur la station de destination
    if (courant == id_dst) {
        printf("\nTrame reçue par la station destination [%zu] :\n", id_dst);
        afficherTrameUser(trame, 46); // ou longueur réelle
        return;
    }

    // Si on est sur un switch, mettre à jour la table de commutation
    if (r->equipements[courant].type == EQUIPEMENT_SWITCH) {
        printf("→ Passage par Switch %zu : ", courant);
        afficherMac(r->equipements[courant].equipement.sw.adresseMac);
        ajouter_commutation(&r->equipements[courant].equipement.sw, trame->adresse_source, 0);
        ajouter_commutation(&r->equipements[courant].equipement.sw, trame->adresse_destination, 1);
    }

    // Parcourir les voisins dans le graphe
    for (size_t i = 0; i < r->graphe.nb_aretes; i++) {
        arete a = r->graphe.aretes[i];
        size_t voisin = SIZE_MAX;
        if (a.s1 == courant) voisin = a.s2;
        else if (a.s2 == courant) voisin = a.s1;
        else continue;

        printf("Appel récursif de %zu vers %zu\n", courant, voisin);
        envoyer_trame_recursif(r, voisin, id_dst, trame, visites);
    }
}

void envoyer_trame_via_tous_les_switchs(reseau *r, size_t id_src, size_t id_dst, uint8_t *donnees, size_t longueur) {
    if (!r || id_src >= r->nb_equipements || id_dst >= r->nb_equipements) {
        printf("ID invalide pour les équipements.\n");
        return;
    }

    TrameEthernet trame;
    memset(&trame, 0, sizeof(TrameEthernet));
    memset(trame.preambule, 0xAA, 7);
    trame.sfd = 0xAB;
    if (r->equipements[id_src].type == EQUIPEMENT_STATION)
        trame.adresse_source = r->equipements[id_src].equipement.st.mac;
    else
        trame.adresse_source = r->equipements[id_src].equipement.sw.adresseMac;
    if (r->equipements[id_dst].type == EQUIPEMENT_STATION)
        trame.adresse_destination = r->equipements[id_dst].equipement.st.mac;
    else
        trame.adresse_destination = r->equipements[id_dst].equipement.sw.adresseMac;
    trame.type[0] = 0x08;
    trame.type[1] = 0x00;
    memcpy(trame.data, donnees, longueur);
    memset(trame.fcs, 0xDE, 4);

    printf("\n--- Début de l'envoi ---\n");
    printf("Source [%zu] => Destination [%zu]\n", id_src, id_dst);

    int *visites = calloc(r->nb_equipements, sizeof(int));
    envoyer_trame_recursif(r, id_src, id_dst, &trame, visites);
    free(visites);
}