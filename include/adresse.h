#pragma once


#include <stdint.h>



// Structure d'adresse MAC : 6 octets
typedef struct AdresseMac{
    uint8_t octets[6];
} AdresseMac;

// Structure d'adresse IP : 4 octets
typedef struct AdresseIP{
    uint8_t octets[4];
} AdresseIP;

char * mac_to_string(AdresseMac mac, char * str_mac);
int string_to_mac(char* str, AdresseMac *mac);
void afficherMac(AdresseMac m);


char* ip_to_string(AdresseIP ip, char* str_ip);
int string_to_ip(char* str, AdresseIP *ip);
void afficherIp(AdresseIP ip);

int comparer_mac(const AdresseMac mac1, const AdresseMac mac2);
int comparer_mac_lex(const AdresseMac mac1, const AdresseMac mac2);