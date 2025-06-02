#include "../include/adresse.h"
#include <stdio.h>
#include <string.h>


// initialiser adr mac avec les 6 octets données en param
void init_AdresseMac(AdresseMac *mac, uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4, uint8_t o5, uint8_t o6) {
    mac->octets[0] = o1;    // 1er octet
    mac->octets[1] = o2;
    mac->octets[2] = o3;
    mac->octets[3] = o4;
    mac->octets[4] = o5;
    mac->octets[5] = o6;
}

// convertir string en structure AdresseMac
void string_to_mac(AdresseMac *mac, char *str) {
    unsigned int octets[6];

    // lis les octets de la chaine en hexa
    sscanf(str, "%02x:%02x:%02x:%02x:%02x:%02x", &octets[0], &octets[1], &octets[2], &octets[3], &octets[4], &octets[5]);
    
    // copie chaque octet dans la struc
    for (int i = 0; i < 6; i++) {
        mac->octets[i] = (uint8_t)octets[i];
    }
}

// affiche adr mac dans le format xx:xx ...
void afficher_AdresseMac(AdresseMac *mac) {
    printf("%02x:%02x:%02x:%02x:%02x:%02x", mac->octets[0], mac->octets[1], mac->octets[2], mac->octets[3], mac->octets[4], mac->octets[5]);
}

// return true si les deux adr sont identitiques
bool comparer_AdresseMac(AdresseMac *mac1, AdresseMac *mac2) {
    for (int i = 0; i < 6; i++) {
        if (mac1->octets[i] != mac2->octets[i]) {
            return false;
        }
    }
    return true;
}

// compare deux adresses pour le stp
int comparer_AdresseMac_ordre(AdresseMac *mac1, AdresseMac *mac2) {
    for (int i = 0; i < 6; i++) {
        if (mac1->octets[i] < mac2->octets[i])
            return -1;
        else if (mac1->octets[i] > mac2->octets[i])
            return 1;
    }
    return 0;
}


void init_AdresseIP(AdresseIP *ip, uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4) {
    ip->octets[0] = o1;
    ip->octets[1] = o2;
    ip->octets[2] = o3;
    ip->octets[3] = o4;
}

void string_to_ip(AdresseIP *ip, char *str) {
    unsigned int octets[4];
    sscanf(str, "%u.%u.%u.%u", &octets[0], &octets[1], &octets[2], &octets[3]);
    
    for (int i = 0; i < 4; i++) {
        ip->octets[i] = (uint8_t)octets[i];
    }
}

void afficher_AdresseIP(AdresseIP *ip) {
    printf("%u.%u.%u.%u", ip->octets[0], ip->octets[1], ip->octets[2], ip->octets[3]);
}

bool comparer_AdresseIP(AdresseIP *ip1, AdresseIP *ip2) {
    for (int i = 0; i < 4; i++) {
        if (ip1->octets[i] != ip2->octets[i]) {
            return false;
        }
    }
    return true;
}