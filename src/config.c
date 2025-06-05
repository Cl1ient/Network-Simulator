#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SIZE 1024

void lireFichierConfiguration(const char *nomFichier, reseau *reseau) {
    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        printf("Impossible d'ouvrir le fichier %s\n", nomFichier);
        return;
    }

    char ligne[MAX_SIZE];

    size_t nb_equipements, nb_liens;

    //Lecture de la premiere ligne (nb équipements + liens)
    fgets(ligne, MAX_SIZE, fichier);
    sscanf(ligne, "%zu %zu", &nb_equipements, &nb_liens);
    printf("Nombre d'équipements : %zu, Nombre de liens : %zu\n", nb_equipements, nb_liens);

    reseau->nb_equipements = 0;
    reseau->nb_switches=0;

    //Lecture des équipements (switchs et stations)
    size_t i = 0;
    while (i < nb_equipements && fgets(ligne, MAX_SIZE, fichier)) {
        int type;
        char mac_str[18], ip_str[16] = "";
        size_t nb_ports = 0, priorite = 0;

        if (sscanf(ligne, "%d ; %17s ; %zu ; %zu", &type, mac_str, &nb_ports, &priorite) == 4) {
            equipement eq;
            eq.type = EQUIPEMENT_SWITCH;
            eq.equipement.sw = creer_switch(mac_str, nb_ports, priorite);
            reseau->equipements[reseau->nb_equipements++] = eq;
            reseau->nb_switches++;
            afficher_switch(&eq.equipement.sw);
            ajouter_sommet(&reseau->graphe); 

        } else if (sscanf(ligne, "%d ; %17s ; %15s", &type, mac_str, ip_str) == 3) {
            equipement eq;
            eq.type = EQUIPEMENT_STATION;
            eq.equipement.st = creer_station(mac_str, ip_str);
            reseau->equipements[reseau->nb_equipements++] = eq;
            afficher_station(&eq.equipement.st);
            ajouter_sommet(&reseau->graphe);
        }
        printf("\n");
        i++;
    }

    //Lecture des liens
    size_t j = 0;
    while (j < nb_liens && fgets(ligne, MAX_SIZE, fichier)) {
        size_t id1, id2, poids;
        sscanf(ligne, "%zu ; %zu ; %zu", &id1, &id2, &poids);
        connecter_equipement(reseau, id1, id2, poids);
        printf("Connexion ajoutée : %zu ↔ %zu (Poids=%zu)\n", id1, id2, poids);
        j++;
    }

    fclose(fichier);
}
