#include <stdio.h>
#include "include/reseau.h"
#include "include/config.h"
#include "include/switch.h"
#include "include/station.h"
#include "include/adresse.h"
#include "include/algos.h"
#include "include/graphe.h"
#include "include/tram.h"

int main() {
    /*AdresseMac mac;
    AdresseIP ip;

    string_to_mac("01:23:45:67:89:AB", &mac);
    string_to_ip("192.168.1.1", &ip);

    printf("Adresse MAC convertie : \n");
    afficherMac(mac);
    printf("\n");

    printf("Adresse IP convertie : \n");
    afficherIp(ip);
    printf("\n");

    station station = creer_station("02:34:56:78:9A:BC", "192.168.1.2");
    printf("Station creee :\n");
    afficher_station(&station);
    printf("\n");

    Switch sw = creer_switch("AA:BB:CC:DD:EE:FF", 4, 10);
    printf("Switch cree :\n");
    afficher_switch(&sw);
    printf("\n");

    graphe g;
    init_graphe(&g);

    ajouter_sommet(&g);
    ajouter_sommet(&g);
    ajouter_arete(&g, (arete){0, 1, 5});

    printf("Graphe cree :\n");
    afficher(&g);
    printf("\n");

    reseau reseau = creer_reseau();
    ajouter_station(&reseau, "01:23:45:67:89:AB", "192.168.1.1");
    ajouter_switch(&reseau, "AA:BB:CC:DD:EE:FF", 4, 10);
    connecter_equipement(&reseau, 0, 1, 5);

    printf("Réseau complet :\n");
    afficher_reseau(&reseau);
    printf("\n");

    // Libérer la mémoire des stations
    for (size_t i = 0; i < reseau.nb_stations; i++) {
        free(reseau.stations[i].mac.octets);
    }
    free(reseau.stations);

    // Libérer la mémoire des switches
    for (size_t i = 0; i < reseau.nb_switch; i++) {
        free(reseau.switchs[i].adresseMac.octets);
    }
    free(reseau.switchs);

    // Libérer la structure du graphe
    deinit_graphe(&reseau.graphe);*/

    reseau reseau = creer_reseau();
    lireFichierConfiguration("mylan_no_cycle.lan", &reseau);

    TrameEthernet trame = {
        .preambule = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},
        .sfd = 0xab,
        .adresse_destination = {0x02, 0x46, 0x8a, 0xce, 0x13, 0x57},
        .adresse_source = {0xde, 0xad, 0xbe, 0xef, 0xca, 0xfe},
        .type = {0x08, 0x06},
        .data = {
            0x45, 0x00, 0x00, 0x3c, 0x1c, 0x46, 0x40, 0x00, 0x40, 0x06, 0xb1, 0xe6, 0xc0, 0xa8, 0x00, 0x68,
            0xc0, 0xa8, 0x00, 0x01, 0x00, 0x50, 0x04, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x60, 0x02, 0x20, 0x00,
            0x56, 0xd7, 0x00, 0x00, 0x02, 0x04, 0x05, 0xb4, 0x01, 0x03, 0x03, 0x07, 0x01, 0x01, 0x08, 0x0a,
            0x0a, 0xf1, 0xa2, 0x5c, 0x00, 0x00, 0x00, 0x00
        },
        .fcs = {0x9a, 0xbc, 0xde, 0xf0}
    };

    size_t data_length = calculerDataLength(trame.data, 1500);
    afficherTrameUser(&trame, data_length);
    afficherTrameHexa(&trame, data_length);


    return 0;
}
