//
// Created by alexis on 5/22/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/adresse.h"


char * mac_to_string(AdresseMac M, char * str_mac){
    sprintf(str_mac, "%02X:%02X:%02X:%02X:%02X:%02X",M.octets[0], M.octets[1], M.octets[2],M.octets[3], M.octets[4], M.octets[5]);
    return str_mac; // return du pointeur pour utilisation direct
}

// Convertit une chaîne en adresse MAC, retourne 1 si la conversion est ok, sinon 0
int string_to_mac(char *str, AdresseMac *M) {
    return sscanf(str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &M->octets[0], &M->octets[1], &M->octets[2], &M->octets[3], &M->octets[4], &M->octets[5]) == 6;
}

void afficherMac(AdresseMac mac){
    char taille[18]; // taille min du mac sous forme string
    printf("MAC = %s\n", mac_to_string(mac, taille));
}


// afficher une adresse ip :
char* ip_to_string(AdresseIP ip, char* str_ip) {
    sprintf(str_ip, "%d.%d.%d.%d", ip.octets[0], ip.octets[1], ip.octets[2], ip.octets[3]);
    return str_ip;
}

int string_to_ip(char *str, AdresseIP *ip) {
    return sscanf(str, "%hhd.%hhd.%hhd.%hhd", &ip->octets[0], &ip->octets[1], &ip->octets[2], &ip->octets[3]) == 4;
}


void afficherIp(AdresseIP ip) {
    char str_ip[16];
    ip_to_string(ip, str_ip);
    printf("Adresse IP : %s\n", str_ip);
}

