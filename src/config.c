#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config.h"

void lireLigneEquipement(char *ligne, int numero) {
    int i = 0, j = 0, champ = 0;
    char typeChar[10], champ1[50], champ2[50], champ3[50];

    // Initialiser toutes les chaînes
    typeChar[0] = champ1[0] = champ2[0] = champ3[0] = '\0';

    while (ligne[i] != '\0' && ligne[i] != '\n') {
        if (ligne[i] == ';') {
            champ++;
            j = 0;
        } else {
            if (champ == 0) {
                typeChar[j] = ligne[i];
                typeChar[j + 1] = '\0';
            } else if (champ == 1) {
                champ1[j] = ligne[i];
                champ1[j + 1] = '\0';
            } else if (champ == 2) {
                champ2[j] = ligne[i];
                champ2[j + 1] = '\0';
            } else if (champ == 3) {
                champ3[j] = ligne[i];
                champ3[j + 1] = '\0';
            }
            j++;
        }
        i++;
    }

    int type = typeChar[0] - '0';

    if (type == 2) {
        printf("Switch [%d]\n", numero);
        printf("  MAC      : %s\n", champ1);
        printf("  Ports    : %s\n", champ2);
        printf("  Priorité : %s\n\n", champ3);
    } else if (type == 1) {
        printf("Station [%d]\n", numero);
        printf("  MAC : %s\n", champ1);
        printf("  IP  : %s\n\n", champ2);
    } else {
        printf("Équipement inconnu (type=%d)\n", type);
    }
}

// Fonction pour afficher une ligne de lien
void lireLigneLien(char *ligne, int numero) {
    int i = 0;
    char champ1[10], champ2[10], champ3[10];
    int champ = 0;
    int j = 0;

    champ1[0] = champ2[0] = champ3[0] = '\0';

    while (ligne[i] != '\0' && ligne[i] != '\n') {
        if (ligne[i] == ';') {
            champ++;
            j = 0;
        } else {
            if (champ == 0) {
                champ1[j] = ligne[i];
                champ1[j + 1] = '\0';
            } else if (champ == 1) {
                champ2[j] = ligne[i];
                champ2[j + 1] = '\0';
            } else if (champ == 2) {
                champ3[j] = ligne[i];
                champ3[j + 1] = '\0';
            }
            j++;
        }
        i++;
    }

    printf("Lien %d : %s <-> %s (poids : %s)\n", numero, champ1, champ2, champ3);
}

void lireFichierConfiguration(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", nomFichier);
        return;
    }

    char ligne[100];
    int nbEquipements = 0, nbLiens = 0;

    // Lire la première ligne (en-tête)
    fgets(ligne, sizeof(ligne), fichier);
    sscanf(ligne, "%d %d", &nbEquipements, &nbLiens);

    printf("Nombre d'équipements : %d\n", nbEquipements);
    printf("Nombre de liens : %d\n\n", nbLiens);

    // Lire les équipements
    printf("=== ÉQUIPEMENTS ===\n");
    int i = 0;
    while (i < nbEquipements) {
        if (fgets(ligne, sizeof(ligne), fichier) != NULL) {
            lireLigneEquipement(ligne, i);
        }
        i = i + 1;
    }

    // Lire les liens
    printf("=== LIENS ===\n");
    i = 0;
    while (i < nbLiens) {
        if (fgets(ligne, sizeof(ligne), fichier) != NULL) {
            lireLigneLien(ligne, i);
        }
        i = i + 1;
    }

    fclose(fichier);
}
