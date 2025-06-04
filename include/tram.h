#pragma once

#include <stddef.h>
#include "adresse.h"
#include <stdint.h>

#define TRAME_PREAMBULE_SIZE 7
#define TRAME_SFD_SIZE 1
#define TRAME_ADDR_SIZE 6
#define TRAME_TYPE_SIZE 2
#define TRAME_DATA_MIN_SIZE 46
#define TRAME_DATA_MAX_SIZE 1500
#define TRAME_FCS_SIZE 4

#define TYPE_IPv4 0x0800
#define TYPE_ARP 0x0806
#define TYPE_IPv6 0x86DD

typedef struct {
    uint8_t preambule[TRAME_PREAMBULE_SIZE]; 
    uint8_t sfd;           
    AdresseMac adresse_destination;             
    AdresseMac adresse_source;                      
    uint16_t type;                             
    uint8_t data[TRAME_DATA_MAX_SIZE];         
    size_t data_size;                      
    uint32_t fcs;                         
} TrameEthernet;

void init_trame_ethernet(TrameEthernet *trame, AdresseMac *source, AdresseMac *dest, uint16_t type);
void set_trame_data(TrameEthernet *trame, uint8_t *data, size_t size);
void afficher_trame_utilisateur(TrameEthernet *trame);
void afficher_trame_hexa(TrameEthernet *trame);
size_t taille_trame_complete(TrameEthernet *trame);

