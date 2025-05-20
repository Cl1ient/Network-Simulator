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

void creationStation();