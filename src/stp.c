//
// Created by acelya on 28/05/25.
//

#include "../include/stp.h"

#include <stdlib.h>
#include <string.h>

static portSTP **table_ports_stp = NULL;
static STP *etats_switchs = NULL;

static int comparer_bpdu(AdresseMac r1, int c1, AdresseMac t1, AdresseMac r2, int c2, AdresseMac t2) {
    int cmp = comparer_adresses_mac(r1, r2);
    if (cmp != 0) return cmp;
    if (c1 != c2) return c1 - c2;
    return comparer_adresses_mac(t1, t2);
}

void initialiser_stp(Reseau *r) {
    int n = r->nbSwitchs;
    table_ports_stp = malloc(sizeof(PortSTP*) * n);
    etats_switchs = malloc(sizeof(EtatSwitchSTP) * n);

    for (int i = 0; i < n; i++) {
        int nb_ports = r->switchs[i].nb_ports;
        table_ports_stp[i] = malloc(sizeof(PortSTP) * nb_ports);
        for (int p = 0; p < nb_ports; p++) {
            table_ports_stp[i][p].etat = STP_INCONNU;
            table_ports_stp[i][p].cout = 1;
            memset(&table_ports_stp[i][p].voisin, 0, sizeof(AdresseMac));
        }

        etats_switchs[i].rootID = r->switchs[i].adresseMac;
        etats_switchs[i].cout_vers_racine = 0;
        etats_switchs[i].port_racine = -1;
    }
}

// À compléter plus tard avec un algo de convergence STP
void lancer_stp(Reseau *r) {
    // Étapes :
    // 1. Élire la racine (switch avec MAC la plus petite)
    // 2. Propager les BPDU (RootID, Cost, TransmittingID)
    // 3. Déterminer pour chaque switch : port racine, ports désignés, ports bloqués

    // (À implémenter après la structure du graphe / connexions)
}

void afficher_etat_stp(Reseau *r) {
    for (int i = 0; i < r->nbSwitchs; i++) {
        printf("Switch %d [%s] : racine = %s, coût = %d, port racine = %d\n",
               i,
               adresse_mac_to_str(r->switchs[i].adresseMac),
               adresse_mac_to_str(etats_switchs[i].rootID),
               etats_switchs[i].cout_vers_racine,
               etats_switchs[i].port_racine
        );

        for (int p = 0; p < r->switchs[i].nb_ports; p++) {
            const char* etat_str = "";
            switch (table_ports_stp[i][p].etat) {
                case STP_INCONNU: etat_str = "INCONNU"; break;
                case STP_RACINE:  etat_str = "RACINE"; break;
                case STP_DESIGNE: etat_str = "DESIGNE"; break;
                case STP_BLOQUE:  etat_str = "BLOQUE"; break;
            }

            printf("  Port %d : %s (voisin %s)\n", p, etat_str,
                   adresse_mac_to_str(table_ports_stp[i][p].voisin));
        }
    }
}
