//
// Created by alexis on 5/22/25.
//

#ifndef STATION_H
#define STATION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"
#include "adresse.h"


typedef struct station {
    AdresseMac mac;
    AdresseIP ip;
    sommet sommet;

} station;

void creationStation(AdresseMac mac, AdresseIP ip, sommet s);
void afficherStation(station s);


#endif //STATION_H
