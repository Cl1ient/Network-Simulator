//
// Created by alexis on 5/22/25.
//

#ifndef SWITCH_H
#define SWITCH_H
#include <stdio.h>
#include "adresse.h"
#include "graphe.h"


// entrée dans la table de commutation
typedef struct entreeTable{
    AdresseMac adresse;    // Adresse MAC connue
    int port;       // Port associé à l'adresse
} entreeTable;

// table de commu complète, utilisé dans chaque switch
typedef struct tableCommutation{
    entreeTable *entrees;  // Liste des entrées
    int taille;  // Nombre d’entrées valides dans la table
} tableCommutation;

// represente un switch d'un réseau
typedef struct Switch{
    AdresseMac adresseMac;     // adr mac unique du switch
    int nbPort;     // nb tot de ports du switch
    int priorite;       // priorite pour le stp
    tableCommutation tblCommu;      // tbl commu
    sommet sommet;     // ref vers le sommet du graphe associé a ce switch
} Switch;

// créer un switch
//switch creationSwitch(Mac mac, int nbPort, int priorite, sommet* sommet);

// ajt une entrée à la table de commu
//void ajtEntreeTable(TableCommutation* table, AdresseMac adresse, int port);

// Recherche une adresse MAC dans la table de commu, return port associé si trouvé, sinon -1
//int rechercherMac(TableCommutation* table, Mac adresse);



#endif //SWITCH_H
