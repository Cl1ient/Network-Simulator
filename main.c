#include <stdio.h>
#include "include/reseau.h"
#include "include/switch.h"
#include "include/station.h"
#include "include/adresse.h"
#include "include/algos.h"
#include "include/graphe.h"

int main() {
    AdresseMac mac;
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

    return 0;
}
