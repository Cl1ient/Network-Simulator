#include "src/adresse.c"
#include "include/adresse.h"

int main(){
    char buffer[18]; // taille min du mac sous forme string
    AdresseMac mac = {{0x54, 0xD6, 0xA6, 0x82, 0xC5, 0x23}};
    printf("MAC = %s\n", mac_to_string(mac, buffer));
}