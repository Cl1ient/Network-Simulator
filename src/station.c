#include "../include/station.h"
#include <stdio.h>

void init_station(station *st, AdresseMac *mac, AdresseIP *ip) {
    st->mac = *mac;
    st->ip = *ip;
}

void afficher_station(station *st) {
    printf("Station - MAC: ");
    afficher_AdresseMac(&st->mac);
    printf(", IP: ");
    afficher_AdresseIP(&st->ip);
    printf("\n");
}

void station_envoyer_trame(station *st, AdresseMac *dest, uint16_t type, uint8_t *data, size_t data_size, trame_ethernet *trame) {
    init_trame_ethernet(trame, &st->mac, dest, type);
    set_trame_data(trame, data, data_size);
    
    printf("Station ");
    afficher_AdresseMac(&st->mac);
    printf(" envoie une trame vers ");
    afficher_AdresseMac(dest);
    printf("\n");
}

void station_recevoir_trame(station *st, trame_ethernet *trame) {
    printf("Station ");
    afficher_AdresseMac(&st->mac);
    printf(" reçoit une trame de ");
    afficher_AdresseMac(&trame->source);
    printf("\n");
}