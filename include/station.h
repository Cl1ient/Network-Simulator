//
// Created by alexis on 5/22/25.
//

#pragma once

#include "adresse.h"


typedef struct station {
    AdresseMac mac;
    AdresseIP ip;
} station;

station creer_station(char* mac_str, char* ip_str);
void afficher_station(station* s);

