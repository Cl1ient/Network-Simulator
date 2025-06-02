#include <stdio.h>
#include <string.h>
#include "include/reseau.h"
#include "include/config.h"
#include "include/trame.h"

void test_reseau_simple() {
    printf("\n=== TEST RÉSEAU SIMPLE ===\n");

    reseau r;
    init_reseau(&r);

    // Création adresses MAC/IP
    AdresseMac mac_sw1, mac_st1, mac_st2;
    AdresseIP ip_st1, ip_st2;

    string_to_mac(&mac_sw1, "01:45:23:a6:f7:ab");
    string_to_mac(&mac_st1, "54:d6:a6:82:c5:23");
    string_to_mac(&mac_st2, "c8:69:72:5e:43:af");

    string_to_ip(&ip_st1, "130.79.80.21");
    string_to_ip(&ip_st2, "130.79.80.27");

    // Ajout équipements
    int sw1 = ajouter_switch(&r, &mac_sw1, 8, 1024);
    int st1 = ajouter_station(&r, &mac_st1, &ip_st1);
    int st2 = ajouter_station(&r, &mac_st2, &ip_st2);

    // Ajout liens
    ajouter_lien(&r, sw1, st1, 4);
    ajouter_lien(&r, sw1, st2, 19);

    // Affichage réseau initial
    afficher_reseau(&r);

    // Message à envoyer
    char *message = "Hello World!";
    size_t len = strlen(message);

    printf("\n=== SIMULATION ENVOI TRAME ===\n");

    // Appel de ta fonction qui simule l'envoi (déjà gère la propagation)
    simuler_envoi_trame(&r, st1, &mac_st2, TYPE_IPv4, (uint8_t *)message, len);

    // Affichage table de commutation du switch après l’envoi
    if (r.equipements[sw1].type == EQUIPEMENT_SWITCH) {
        printf("\n=== TABLE DE COMMUTATION DU SWITCH ===\n");
        afficher_table_commutation(&r.equipements[sw1].equipement.sw);
    }

    deinit_reseau(&r);
}


void echange_trame(reseau *r) {
    printf("\n=== ECHANGES DE TRAMES ===\n");

    int station1 = -1, station2 = -1;       
    // on cherche deux stations pour simuler l'envoie de trames
    for (int i = 0; i < r->nb_equipements; i++) {
        if (r->equipements[i].type == EQUIPEMENT_STATION) {
            if (station1 == -1) {
                station1 = i;
            } else {
                station2 = i;
                break;
            }
        }
    }

    station *src = &r->equipements[station1].equipement.st;
    station *dest = &r->equipements[station2].equipement.st;

    trame_ethernet trame;
    init_trame_ethernet(&trame, &src->mac, &dest->mac, TYPE_IPv4);

    char *data = "HELLO!";
    set_trame_data(&trame, (uint8_t*)data, strlen(data));

    afficher_trame_utilisateur(&trame);
    printf("\n");
    afficher_trame_hexa(&trame);

    simuler_envoi_trame(r, station1, &dest->mac, TYPE_IPv4, (uint8_t*)data, strlen(data));
}

void lecture_fichier_config(reseau *r) {
    printf("\n=== TEST FICHIER CONFIGURATION ===\n");

    lireFichierConfiguration("fichierTest.txt", r);
    afficher_reseau(r);
}

int main() {
    printf("=== SIMULATEUR DE RÉSEAU LOCAL ===\n");

    //test_reseau_simple();

    reseau r;

    //lecture du fichier
    lecture_fichier_config(&r);

    printf("\n");

    //simulation d'échange de trames
    echange_trame(&r);

    printf("\n");

    deinit_reseau(&r);

    printf("\n=== FIN ===\n");
    return 0;
}