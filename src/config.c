#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lireFichierConfiguration(char *nomFichier, reseau *r) {
    FILE *fichier = fopen(nomFichier, "r");     // ouvre le fichier en lecture
    if (!fichier) {
        printf("Erreur: impossible d'ouvrir le fichier %s\n", nomFichier);
        return;
    }

    init_reseau(r);     // initialise un reseau, vide

    char ligne[1024];

    // lecture de la première ligne (nb equ + liens)
    size_t nb_equipements, nb_liens;
    fgets(ligne, 1024, fichier);
    sscanf(ligne, "%zu %zu", &nb_equipements, &nb_liens);
    printf("Nombres d'équipements et de liens : %zu équipements, %zu liens\n", nb_equipements, nb_liens);

    // Lire les équipements
    for (size_t i = 0; i < nb_equipements; i++) {
        int type;
        fgets(ligne, 1024, fichier);  // lire la ligne entière

        char mac_str[18] = "";  // pour stocker l'adresse MAC
        char ip_str[16] = "";   // pour stocker l'adresse IP
        int nb_ports = 0, priorite = 0;

        // on essaie de lire comme switch
        if (sscanf(ligne, "%d ; %17s ; %d ; %d", &type, mac_str, &nb_ports, &priorite) == 4 && type == EQUIPEMENT_SWITCH) {
        AdresseMac mac;
        string_to_mac(&mac, mac_str);
        ajouter_switch(r, &mac, nb_ports, priorite);
        } 

        // Sinon, lecture comme station
        else if (sscanf(ligne, "%d ; %17s ; %15s", &type, mac_str, ip_str) == 3 && type == EQUIPEMENT_STATION) {
            AdresseMac mac;
            AdresseIP ip;
            string_to_mac(&mac, mac_str);
            string_to_ip(&ip, ip_str);
            ajouter_station(r, &mac, &ip);
        }
    }

    // Lire les liens
    for (int i = 0; i < nb_liens; i++) {
        fgets(ligne, 1024, fichier);
        int eq1, eq2, cout;
        sscanf(ligne, "%d;%d;%d", &eq1, &eq2, &cout);

        ajouter_lien(r, eq1, eq2, cout);
    }

    fclose(fichier);
    printf("Configuration chargée avec succès\n");
}