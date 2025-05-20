#include <stdio.h>
#include <stdlib.h>

#include "../include/adresse.h"


char * mac_to_string(AdresseMac M, char * str_mac){
    sprintf(str_mac, "%02X:%02X:%02X:%02X:%02X:%02X",M.octets[0], M.octets[1], M.octets[2],M.octets[3], M.octets[4], M.octets[5]);
    return str_mac; // return du pointeur pour utilisation direct
}