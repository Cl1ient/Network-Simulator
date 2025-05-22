#ifndef ADRESSE_H
#define ADRESSE_H
#include <stdio.h>

// Structure d'adresse MAC : 6 octets
typedef struct {
    unsigned char octets[6];
} AdresseMac;

// Structure d'adresse IP : 4 octets
typedef struct {
    unsigned char octets[4];
} AdresseIP;

char * mac_to_string(AdresseMac M, char * str_mac);

void afficherIp(AdresseIP ip);
void afficherMac(AdresseMac m);
char* ip_to_string(AdresseIP ip, char* str_ip);
#endif //ADRESSE_H
