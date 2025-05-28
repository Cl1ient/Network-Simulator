#include "../include/tram.h"
#include "../include/adresse.h"
#include <stdio.h>
#include <stdint.h>


size_t calculerDataLength(uint8_t *data, size_t max_length) {
    size_t length = 0;
    size_t zeroCount = 0;
    for (size_t i = 0; i < max_length; i++) {
        if (data[i] == 0) {
            zeroCount++;
        } else {
            zeroCount = 0;
        }

        if (zeroCount >= 5) {
            length -= (zeroCount - 1); // ne pas compte la séquence de 0
            break;
        }
        length++;
    }
    return length;
}

// Afficher en mode utilisateur
void afficherTrameUser(TrameEthernet *trame, int dataLength) {
    printf("Préambule : ");
    for (int i = 0; i < 7; i++) {
        printf("%02x ", trame->preambule[i]);
    }

    printf("\nSFD : %02x\n", trame->sfd);
    printf("Adresse de destination => ");

    afficherMac(trame->adresse_destination);
    printf("\nAdresse source => ");
    afficherMac(trame->adresse_source);

    printf("\nType : 0x%02x%02x\n", trame->type[0], trame->type[1]);

    printf("Données : \n");
    for (int i = 0; i < dataLength; i++) {
        printf("%02x ", trame->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    printf("FCS : %02x%02x%02x%02x\n", trame->fcs[0], trame->fcs[1], trame->fcs[2], trame->fcs[3]);
}

// Fonction pour afficher une trame Ethernet en hexadécimal
void afficherTrameHexa(TrameEthernet *trame, size_t dataLength) {
    int taille_trame = 7 + 1 + 6 + 6 + 2 + dataLength; // calluc dynamique de la taille de la tram
    uint8_t *octets = (uint8_t*)trame;

    for (int i = 0; i < taille_trame; i++) {
        printf("%02x ", octets[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}
