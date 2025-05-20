#include "adresse.h"
#include


// entrée dans la table de commutation
typedef struct {
    Mac adresse;    // Adresse MAC connue
    int port;       // Port associé à l'adresse
} EntreeTable;

// table de commu complète, utilisé dans chaque switch
typedef struct {
    EntreeTable entrees[TAILLE_TABLE_COMMUTATION];  // Liste des entrées (table statique)
    int taille;  // Nombre actuel d’entrées valides dans la table
} TableCommutation;

// represente un switch d'un réseau
typedef struct {
    Mac adresseMac;     // adr mac unique du switch
    int nbPort;     // nb tot de ports du switch
    int priorite;       // priorite pour le stp
    TableCommutation tblCommu;      // tbl commu
    Sommet* sommet;     // ref vers le sommet du graphe associé a ce switch
} Switch

// créer un switch
//Switch creationSwitch(Mac mac, int nbPort, int priorite, Sommet* sommet);

// ajt une entrée à la table de commu
//void ajtEntreeTable(TableCommutation* table, Mac adresse, int port);

// Recherche une adresse MAC dans la table de commu, return port associé si trouvé, sinon -1
//int rechercherMac(TableCommutation* table, Mac adresse);


