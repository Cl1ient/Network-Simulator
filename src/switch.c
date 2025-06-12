//
// Created by acelya on 23/05/2025.
//

#include "stdio.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/adresse.h"
#include "../include/switch.h"
#include "../include/station.h"
#include "../include/reseau.h"

Switch_s creer_switch(char* mac_str, size_t nb_ports, int prio) {
    Switch_s sw;

    if (!string_to_mac(mac_str, &sw.adresseMac)) {
        printf("Erreur: mac invalide\n");
        exit(EXIT_FAILURE); 
    }

    sw.nb_ports = nb_ports;
    sw.priorite = prio;
    sw.nb_asso = 0;

    // Initialiser la table de commutation
    for (size_t i = 0; i < MAX_ENTREES_COMMUTATION; i++) {
        sw.table_commutation[i].valide = 0;
    }

    // ALLOCATION pour tableau des états des ports
    sw.etat_ports = malloc(nb_ports * sizeof(EtatPort));
    if (!sw.etat_ports) {
        fprintf(stderr, "Erreur malloc etat_ports\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < nb_ports; i++) {
        sw.etat_ports[i] = PORT_BLOQUE;  // par défaut
    }

    // Initialiser du BPDU (il se considère racine au début)
    sw.meilleur_bpdu.mac = sw.adresseMac;
    sw.meilleur_bpdu.priorite = sw.priorite;
    sw.meilleur_bpdu.cout = 0;
    sw.meilleur_bpdu.port = -1;

    sw.est_racine = 1;
    sw.port_racine = -1;

    return sw;
}


// ajt d'une entrée dans la table
void ajouter_commutation (Switch_s* sw, AdresseMac mac, size_t port) {
    if (!sw) {
        return;
    }

    if (sw->nb_asso >= MAX_ENTREES_COMMUTATION) {
        printf("Table de commutation pleine\n");
        return;
    }

    // Si la mac y est déja, on ne la rajoute pas.
    for (int i = 0; i < sw->nb_asso; i++) {
        if (comparer_mac(sw->table_commutation[i].mac, mac) && sw->table_commutation[i].port == port) {
            return;
        }
    }

    sw->table_commutation[sw->nb_asso].mac = mac;
    sw->table_commutation[sw->nb_asso].port = port;
    sw->table_commutation[sw->nb_asso].valide = 1;
    sw->nb_asso++;

}

void afficher_switch(Switch_s* sw) {
    printf("Switch: \n");
    afficherMac(sw->adresseMac);
    printf("Nb de ports : %zu\n", sw->nb_ports);
    printf("Prio STP : %d\n", sw->priorite);
    afficher_table_commutation(sw);

}

void afficher_table_commutation(Switch_s *sw) {
    printf("Table de commutation du switch (MAC → port) :\n");
    for (size_t i = 0; i < sw->nb_ports; i++) {
        if (sw->table_commutation[i].valide) {
            char mac_str[18];
            mac_to_string(sw->table_commutation[i].mac, mac_str);
            printf("MAC %s → Port %zu\n", mac_str, sw->table_commutation[i].port);
        }
    }

}


// comparaison de BPDU selon l'ordre de priorité STP.
// priorité > MAC > coût > port.
// Retourne un entier négatif, nul ou positif selon que 'a' est "inférieur", "égal" ou "supérieur" à 'b'.
int comparer_bpdu(BPDU *a, BPDU *b) {
    if (a->priorite != b->priorite) {
        return a->priorite - b->priorite;
    }

    int cmp_mac = comparer_mac_lex(a->mac, b->mac);
    if (cmp_mac != 0) {
        return cmp_mac;
    }

    if (a->cout != b->cout) {
        return a->cout - b->cout;
    }

    return a->port - b->port;
}


// applique STP
// comparant et échange des BPDU, puis met à jour l’état des ports
void appliquer_stp(reseau *r) {
    int stable = 0;     // si les états de ports se sont stabilisés.

    while (!stable) {
        stable = 1;     // on suppose que c'est stable au début

        // Calcul des BPDU pour chaque switch
        for (int i = 0; i < r->nb_equipements; i++) {

            // on ignore les stations
            if (r->equipements[i].type != EQUIPEMENT_SWITCH) {
                continue;
            }

            Switch_s *sw = &r->equipements[i].equipement.sw;

            // Si le switch se considère comme racine, il construit sont propre BPDU
            if (sw->est_racine) {
                sw->meilleur_bpdu.mac = sw->adresseMac;
                sw->meilleur_bpdu.priorite = sw->priorite;
                sw->meilleur_bpdu.cout = 0;
                sw->meilleur_bpdu.port = -1;
            }

            // Récupère tous les voisins (ports connectés au switch dans le graphe)
            sommet voisins[64];
            size_t nb_voisins = sommets_adjacents(&r->graphe, i, voisins);

            // on parcours tout les voisins
            for (size_t port = 0; port < sw->nb_ports && port < nb_voisins; port++) {
                int voisin = voisins[port];
                equipement *eq_voisin = &r->equipements[voisin];

                // si c'est une station, on met en désigné car tjr accessible
                if (eq_voisin->type == EQUIPEMENT_STATION) {
                    sw->etat_ports[port] = PORT_ACTIF_DESIGNE;
                    continue;
                }

                // sinn on traite l'autre switch comme un emmetteur de bpdu
                Switch_s *sw_voisin = &eq_voisin->equipement.sw;

                // on recupere le bpdu du voisin et on ajoute le cout du lien
                BPDU bpdu_voisin = sw_voisin->meilleur_bpdu;
                bpdu_voisin.cout += 1;
                bpdu_voisin.port = (int)port;

                // on compare le bpdu avec le meilleur qu'on a
                int cmp = comparer_bpdu(&bpdu_voisin, &sw->meilleur_bpdu);
                if (cmp < 0) {
                    // BPDU voisin est meilleur, on met à jour
                    sw->meilleur_bpdu = bpdu_voisin;
                    sw->port_racine = (int)port;
                    sw->est_racine = 0;     // ce switch n'est plus racine
                    stable = 0;     // il y a eu un changement donc le reseau n'est pas encore stable
                }
            }
        }

        // Mise à jour des états des ports
        for (int i = 0; i < r->nb_equipements; i++) {
            if (r->equipements[i].type != EQUIPEMENT_SWITCH) {
                continue;
            }

            Switch_s *sw = &r->equipements[i].equipement.sw;

            sommet voisins[64];
            size_t nb_voisins = sommets_adjacents(&r->graphe, i, voisins);

            for (size_t port = 0; port < sw->nb_ports && port < nb_voisins; port++) {
                int voisin = voisins[port];
                equipement *eq_voisin = &r->equipements[voisin];

                if (eq_voisin->type == EQUIPEMENT_STATION) {
                    sw->etat_ports[port] = PORT_ACTIF_DESIGNE;
                    continue;
                }

                Switch_s *sw_voisin = &eq_voisin->equipement.sw;

                // trouve le port du voisin qui mène a ce switch
                int port_voisin = get_port_vers_equipement(&r->graphe, voisin, i);
                if (port_voisin == -1) {
                    // si il y a pas de port valide, on active le port comme désigné par dégaut
                    sw->etat_ports[port] = PORT_ACTIF_DESIGNE; // par défaut
                    continue;
                }

                // recup l'etat du port du voisin
                EtatPort etat_voisin = sw_voisin->etat_ports[port_voisin];

                if ((int)port == sw->port_racine) {
                    sw->etat_ports[port] = PORT_ACTIF_RACINE;
                } else if (etat_voisin == PORT_ACTIF_RACINE || etat_voisin == PORT_BLOQUE) {
                    sw->etat_ports[port] = PORT_ACTIF_DESIGNE;
                } else if (etat_voisin == PORT_ACTIF_DESIGNE) {
                    sw->etat_ports[port] = PORT_BLOQUE;
                } else {
                    sw->etat_ports[port] = PORT_ACTIF_DESIGNE; // défaut
                }
            }
        }
    }
}

int get_port_vers_equipement(const graphe *g, int de_idx, int a_idx) {
    sommet sa[64];
    size_t nb = sommets_adjacents(g, de_idx, sa);

    for (size_t i = 0; i < nb; i++) {
        if (sa[i] == a_idx) {
            return (int)i; // port vers a_idx
        }
    }
    return -1;
}



void afficher_bpdu(BPDU *bpdu) {
    printf("Root MAC: ");
    afficherMac(bpdu->mac);
    printf(", Priorité: %d, Coût: %d, Port: %d\n", bpdu->priorite, bpdu->cout, (int)bpdu->port);
}

void afficher_etat_port(EtatPort etat) {
    switch (etat) {
        case PORT_BLOQUE: printf("Bloqué"); break;
        case PORT_ACTIF_RACINE: printf("Racine"); break;
        case PORT_ACTIF_DESIGNE: printf("Désigné"); break;
        default: printf("Inconnu"); break;
    }
}

void afficher_stp(reseau *r) {
    printf("=== État STP du réseau ===\n");
    for (int i = 0; i < r->nb_equipements; i++) {
        equipement *eq = &r->equipements[i];
        if (eq->type != EQUIPEMENT_SWITCH) continue;

        Switch_s *sw = &eq->equipement.sw;
        printf("Switch %d - MAC: ", i);
        afficherMac(sw->adresseMac);
        printf(" - Est racine: %s\n", sw->est_racine ? "Oui" : "Non");
        printf("Meilleur BPDU reçu: ");
        afficher_bpdu(&sw->meilleur_bpdu);

        printf("Ports (%zu):\n", sw->nb_ports);
        for (size_t p = 0; p < sw->nb_ports; p++) {
            printf("  Port %zu: ", p);
            afficher_etat_port(sw->etat_ports[p]);
            printf("\n");
        }
        printf("\n");
    }
}
