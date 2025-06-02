/*
Tout ce qui est lié à la gestion d'adresses du réseau
Gère l'initialisation, la lecture depuis un string, l'affichage, la comparaison
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>

// adresse MAC (6 octets)
typedef struct {
    uint8_t octets[6];
} AdresseMac;

// adresse IP (4 octets)
typedef struct {
    uint8_t octets[4];
} AdresseIP;

// adresse MAC
void init_AdresseMac(AdresseMac *mac, uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4, uint8_t o5, uint8_t o6);  // o = octet
void string_to_mac(AdresseMac *mac, char *str);
void afficher_AdresseMac(AdresseMac *mac);
bool comparer_AdresseMac(AdresseMac *mac1, AdresseMac *mac2);
int comparer_AdresseMac_ordre(AdresseMac *mac1, AdresseMac *mac2);

// Fonctions pour adresse IP
void init_AdresseIP(AdresseIP *ip, uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4);
void string_to_ip(AdresseIP *ip, char *str);
void afficher_AdresseIP(AdresseIP *ip);
bool comparer_AdresseIP(AdresseIP *ip1, AdresseIP *ip2);