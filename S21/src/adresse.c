#include <stdio.h>
#include <stdlib.h>

#include "../include/adresse.h"


char * mac_to_string(AdresseMac M, char * str_mac){
    sprintf(str_mac, "%02X:%02X:%02X:%02X:%02X:%02X",M.octets[0], M.octets[1], M.octets[2],M.octets[3], M.octets[4], M.octets[5]);
    return str_mac; // return du pointeur pour utilisation direct
}

void afficherMac(AdresseMac mac){
    char taille[18]; // taille min du mac sous forme string
    printf("MAC = %s\n", mac_to_string(mac, taille));
}


// afficher une adresse ip : 
char* ip_to_string(AdresseIP ip, char* str_ip) {
    sprintf(str_ip, "%d.%d.%d.%d", ip.octet[0],ip.octet[1], ip.octet[2]);
    return str_ip;
}


void afficherIp(AdresseIP ip) {
    char str_ip[16];
    ip_to_string(ip, str_ip);
    printf("Adresse IP : %s\n", str_ip);
}