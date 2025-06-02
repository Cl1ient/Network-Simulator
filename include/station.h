#pragma once

#include "adresse.h"
#include "trame.h"

typedef struct {
    AdresseMac mac;
    AdresseIP ip;
} station;

// Fonctions pour les stations
void init_station(station *st, AdresseMac *mac, AdresseIP *ip);
void afficher_station(station *st);
void station_envoyer_trame(station *st, AdresseMac *dest, uint16_t type, uint8_t *data, size_t data_size, trame_ethernet *trame);
void station_recevoir_trame(station *st, trame_ethernet *trame);