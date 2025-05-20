#include <../include/stdio.h>
#include "adresse.h"

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
