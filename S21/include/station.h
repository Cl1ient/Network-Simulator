#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"


typedef struct station {
    AdresseMac mac;
    AdresseIP ip;
    sommet sommet;

} station;

void creationStation();