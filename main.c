#include <stdio.h>
#include "include/reseau.h"
#include "include/config.h"
#include "include/adresse.h"
#include "include/tram.h"
#include "include/switch.h"
#include "include/algos.h"

void envoyer_message(reseau *r, int index_source, int index_dest, const char *msg) {
    AdresseMac *mac_source = &r->equipements[index_source].equipement.st.mac;
    AdresseMac *mac_dest = &r->equipements[index_dest].equipement.st.mac;

    TrameEthernet trame;
    init_trame_ethernet(&trame, mac_source, mac_dest, TYPE_IPv4);
    set_trame_data(&trame, (uint8_t *)msg, strlen(msg));

    printf("\n\n[TRAME de %d à %d] %s\n", index_source, index_dest, msg);
    afficher_trame_utilisateur(&trame);
    afficher_trame_hexa(&trame);

    envoyer_trame(r, index_source, &trame);
}

void liberer_reseau(reseau *r) {
    if (!r) return;

    // Libérer les tableaux etat_ports de chaque switch
    for (int i = 0; i < r->nb_equipements; i++) {
        if (r->equipements[i].type == EQUIPEMENT_SWITCH) {
            Switch_s *sw = &r->equipements[i].equipement.sw;
            if (sw->etat_ports) {
                free(sw->etat_ports);
                sw->etat_ports = NULL;
            }
        }
    }

    // Libérer le graphe (tableau d'arêtes)
    deinit_graphe(&r->graphe);
}

int main() {
    reseau reseau = creer_reseau();
    lireFichierConfiguration("config.lan", &reseau);

    appliquer_stp(&reseau);

    printf("\n\n");

    afficher_stp(&reseau);

    //afficher(&reseau.graphe);

    // Envois de trames
    envoyer_message(&reseau, 9, 12, "Salut 14, ici 7 !");
    //envoyer_message(&reseau, 14, 7, "Bien reçu 7, ici 14 !");
    //envoyer_message(&reseau, 8, 7, "Hello de 8 vers 7 !");

    printf("\n======== TABLES DE COMMUTATION ========\n");
    for (int i = 0; i < reseau.nb_equipements; i++) {
        if (reseau.equipements[i].type == EQUIPEMENT_SWITCH) {
            printf("Switch %d :\n", i);
            afficher_table_commutation(&reseau.equipements[i].equipement.sw);
            printf("\n");
        }
    }

    liberer_reseau(&reseau);

    return 0;

}



/*
TO DO

Y'a un paquet de switches.

Chaque switch pense qu’il est la racine.

À chaque tour, chaque switch regarde ses voisins pour voir s’il y en
a un plus proche de la racine que lui (genre “hey, t’as une racine plus cool que moi ?”).

Si oui, il change d’avis et prend l’info du voisin.

On continue jusqu’à ce que tout le monde pense que le même switch est la racine.

Ensuite on décide qui garde ses ports actifs ou les bloque, pour pas faire de boucle.
*/
