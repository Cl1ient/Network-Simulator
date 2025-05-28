// dijstra


#include "adresse.h"
#include <stdint.h>

// états possible d'un port stp
typedef enum {
    PORT_INCONNU,
    PORT_RACINE,
    PORT_DESIGN,
    PORT_BLOQUE
} EtatPortSTP;

// structure bpdu
typedef struct BPDU {
    AdresseMac root_id; // id racine
    uint16_t cout_chemin;   // coût du chemin vers la racine
    AdresseMac bridge_id;  // identifiant du bridge (celui qui transmet)
    uint16_t port_id; // identifiant du port émetteur
}BPDU;

