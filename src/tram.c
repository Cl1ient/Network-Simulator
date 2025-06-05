#include "../include/tram.h"
#include "../include/adresse.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void init_trame_ethernet(TrameEthernet *trame, AdresseMac *source, AdresseMac *dest, uint16_t type) {
    // Initialiser le préambule (10101010)
    for (int i = 0; i < TRAME_PREAMBULE_SIZE; i++) {
        trame->preambule[i] = 0xAA;
    }
    
    // SFD 10101011
    trame->sfd = 0xAB;
    
    // Adresses
    trame->adresse_destination = *dest;
    trame->adresse_source = *source;
    
    // Type
    trame->type = type;
    
    // Initialiser les données à zéro
    memset(trame->data, 0, TRAME_DATA_MAX_SIZE);
    trame->data_size = 0;
    
    trame->fcs = 0;
}

void set_trame_data(TrameEthernet *trame, uint8_t *data, size_t size) {
    if (size > TRAME_DATA_MAX_SIZE) {
        size = TRAME_DATA_MAX_SIZE;
    }
    
    memcpy(trame->data, data, size);
    trame->data_size = size;
    
    // Si les données sont trop petites, on complete
    if (trame->data_size < TRAME_DATA_MIN_SIZE) {
        memset(trame->data + trame->data_size, 0, TRAME_DATA_MIN_SIZE - trame->data_size);
        trame->data_size = TRAME_DATA_MIN_SIZE;
    }
}

void afficher_trame_utilisateur(TrameEthernet *trame) {
    printf("=== TRAME ETHERNET ===\n\n");
    printf("Source      : ");
    afficherMac(trame->adresse_source);
    printf("\n");
    
    printf("Destination : ");
    afficherMac(trame->adresse_destination);
    printf("\n");
    
    printf("Type        : 0x%04X", trame->type);
    switch (trame->type) {
        case TYPE_IPv4:
            printf(" (IPv4)");
            break;
        case TYPE_ARP:
            printf(" (ARP)");
            break;
        case TYPE_IPv6:
            printf(" (IPv6)");
            break;
        default:
            printf(" (Inconnu)");
    }
    printf("\n");
    
    printf("Donée : %s\n", trame->data);
    printf("======================\n");
}

void afficher_trame_hexa(TrameEthernet *trame) {
    printf("=== TRAME HEXA ===\n");
    
    // Préambule
    printf("Préambule : ");
    for (int i = 0; i < TRAME_PREAMBULE_SIZE; i++) {
        printf("%02X ", trame->preambule[i]);
    }
    printf("\n");
    
    // SFD
    printf("SFD       : %02X\n", trame->sfd);
    
    // Destination
    printf("Dest      : ");
    for (int i = 0; i < 6; i++) {
        printf("%02X ", trame->adresse_destination.octets[i]);
    }
    printf("\n");
    
    // Source
    printf("Source    : ");
    for (int i = 0; i < 6; i++) {
        printf("%02X ", trame->adresse_source.octets[i]);
    }
    printf("\n");
    
    // Type
    printf("Type      : %02X %02X\n", (trame->type >> 8) & 0xFF, trame->type & 0xFF);
    
    // Données (affichage partiel)
    printf("Données   : ");
    size_t affichage_max = trame->data_size > 16 ? 16 : trame->data_size;
    for (size_t i = 0; i < affichage_max; i++) {
        printf("%02X ", trame->data[i]);
    }
    if (trame->data_size > 16) {
        printf("... (%zu octets au total)", trame->data_size);
    }
    printf("\n");
    
    // FCS
    printf("FCS       : %08X\n", trame->fcs);
    printf("==================\n");
}

size_t taille_trame_complete(TrameEthernet *trame) {
    return TRAME_PREAMBULE_SIZE + TRAME_SFD_SIZE + 2 * TRAME_ADDR_SIZE + TRAME_TYPE_SIZE + trame->data_size + TRAME_FCS_SIZE; 
}
