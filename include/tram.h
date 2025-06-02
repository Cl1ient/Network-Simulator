//
// Created by alexis on 5/28/25.
//
#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../include/adresse.h"
#ifndef TRAM_H
#define TRAM_H

typedef struct {
    uint8_t preambule[7];            // 7 octets pour le préambule
    uint8_t sfd;                     // 1 octet pour le SFD
    AdresseMac adresse_destination;
    AdresseMac adresse_source;      // 6 octets pour l'adresse source
    uint8_t type[2];                 // 2 octets pour le type
    uint8_t data[1500];              // Jusqu'à 1500 octets pour les données
    uint8_t fcs[4];                  // 4 octets pour le FCS
} TrameEthernet;

void afficherTrameUser(TrameEthernet *trame, int dataLength);
void afficherTrameHexa(TrameEthernet *trame, size_t dataLength);
size_t calculerDataLength(uint8_t *data, size_t max_length);


#endif //TRAM_H
