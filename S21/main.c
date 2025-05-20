#include "include/adresse.h"

int main() {
    AdresseIP ip_test = {{192, 168, 0, 42}};
    AdresseMac mac_test = {{0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E}};

    afficherIp(ip_test);
    afficherMac(mac_test);

    return 0;
}

