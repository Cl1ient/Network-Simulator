//
// Created by acelya on 23/05/2025.
//

#include "../include/station.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/adresse.h"

// verifie que les adresses sont valide et creer la station
station creer_station(char* mac_str, char* ip_str) {
    station s;

    int mac_ok = string_to_mac(mac_str, &s.mac);    // 1 si ok, 0 sinon

    // convertir les chaines en adresses
    if (!mac_ok) {      // si la conv echoue
        printf("Errezur lors de la conversion, l'adresse mac est invalide\n");
    }

    int ip_ok = string_to_ip(ip_str, &s.ip);

    if (!ip_ok) {
        printf("Erreur lors de la convesion, l'adresse ip est invalide\n");
    }

    return s;
}

void afficher_station(station* station) {
    printf("Station\n");
    afficherMac(station->mac);
    afficherIp(station->ip);
}