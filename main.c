#include <stdio.h>
#include "include/reseau.h"
#include "include/config.h"
#include "include/adresse.h"
#include "include/tram.h"
#include "include/switch.h"

int main() {


    reseau reseau = creer_reseau();
    lireFichierConfiguration("mylan_no_cycle.lan", &reseau);

    // Préparer les adresses source et destination (par ex. stations 3 et 4)
    AdresseMac *mac_source = &reseau.equipements[7].equipement.st.mac;  // station 3 (index 6)
    AdresseMac *mac_dest   = &reseau.equipements[14].equipement.st.mac;  // station 4 (index 7)

    // Initialiser une trame vide
    TrameEthernet trame;
    init_trame_ethernet(&trame, mac_source, mac_dest, TYPE_IPv4);

    // Remplir la trame avec des données
    uint8_t message[] = "Hello Switch Network !";
    set_trame_data(&trame, message, sizeof(message) - 1);  // -1 pour ne pas copier le \0

    // Afficher la trame en format utilisateur (lisible)
    afficher_trame_utilisateur(&trame);

    // Afficher la trame en hexadécimal (plus bas niveau)
    afficher_trame_hexa(&trame);

    // Envoyer la trame depuis la station 3 (index 6)
    envoyer_trame(&reseau, 7, &trame);






    // Afficher les tables de commutation des switches
    for (int i = 0; i < reseau.nb_equipements; i++) {
        if (reseau.equipements[i].type == EQUIPEMENT_SWITCH) {
            printf("Switch %d :\n", i);
            afficher_table_commutation(&reseau.equipements[i].equipement.sw);
            printf("\n");
        }
    }
}