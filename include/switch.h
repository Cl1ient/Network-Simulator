#pragma once

#include "adresse.h"
#include "trame.h"
#include <stdbool.h>

#define MAX_PORTS 32
#define MAX_TABLE_COMMUTATION 256

// etats ports possible pour STP
typedef enum {
    PORT_INCONNU,
    PORT_RACINE,
    PORT_DESIGNE,
    PORT_BLOQUE
} etat_port;

// entrée de la table de commu
typedef struct {
    AdresseMac mac;
    int port;
    bool valide;
} entree_table_commutation;

// switch
typedef struct {
    AdresseMac mac;
    int nb_ports;
    int priorite;
    
    // tbl de commu
    entree_table_commutation table_commutation[MAX_TABLE_COMMUTATION];
    int nb_entrees_table;
    
    // etats des ports (pour STP)
    etat_port etats_ports[MAX_PORTS];
    
    // infos STP
    AdresseMac racine_id;
    int cout_racine;
    int port_racine;
    bool est_racine;
} Switch;

// switchs
void init_switch(Switch *sw, AdresseMac *mac, int nb_ports, int priorite);
void afficher_switch(Switch *sw);
void afficher_table_commutation(Switch *sw);

// table de commutation
void apprendre_adresse(Switch *sw, AdresseMac *mac, int port);
int chercher_port(Switch *sw, AdresseMac *mac);
void vider_table_commutation(Switch *sw);

// Commutation des trames
void switch_recevoir_trame(Switch *sw, trame_ethernet *trame, int port_entree);
void switch_diffuser_trame(Switch *sw, trame_ethernet *trame, int port_exclus);

// STP
void init_stp_switch(Switch *sw);
char* etat_port_to_string(etat_port etat);