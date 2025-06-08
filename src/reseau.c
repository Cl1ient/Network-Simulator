#include "../include/reseau.h"
#include "../include/graphe.h"
#include "../include/algos.h"
#include "../include/tram.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prépare un res vide avec 0 équipement et un graphe initialisé.
reseau creer_reseau() {
    reseau r;   
    r.nb_equipements = 0;   

    init_graphe(&r.graphe);

    return r;
}

// Connexion de deux équipements (ajt d’une arete dans le graphej avec un poids)
// Retourne 'true' si l'ajout s'est bien passé, sinon 'false'.
bool connecter_equipement(reseau *reseau, size_t id1, size_t id2, int poids) {
    
    // si le pointeur est nul, reseau invalide, impossible de lier
    if (!reseau) {
        return false;
    }

    // creation d'une arete pour le graphe entre les deux equipements
    arete nouvelle_arete = {id1, id2, poids};

    // ajoute l'arete au graphe
    return ajouter_arete(&reseau->graphe, nouvelle_arete);
}



void afficher_reseau(reseau *reseau) {
    if (!reseau) {
        printf("Réseau invalide\n");
        return;
    }

    printf("\n\n=== Réseau === \n\n");

    printf("Équipements : %d\n\n", reseau->nb_equipements);

    for (int i = 0; i < reseau->nb_equipements; i++) {
        if (reseau->equipements[i].type == EQUIPEMENT_STATION) {
            afficher_station(&reseau->equipements[i].equipement.st);
        }
        else if (reseau->equipements[i].type == EQUIPEMENT_SWITCH) {
            afficher_switch(&reseau->equipements[i].equipement.sw);
        }
        printf("\n\n");
    }
    printf("\nStructure du graphe :\n\n");
    afficher(&reseau->graphe);
}
















size_t port_entre_de_precedent(reseau *r, size_t id_switch, size_t id_voisin) {
    sommet voisins[32];     // Tableau pour stocker les voisins du switch
    size_t nb_voisins = sommets_adjacents(&r->graphe, id_switch, voisins);  // Récupère les voisins
    for (size_t i = 0; i < nb_voisins; i++) {   // Parcourt les voisins
        if (voisins[i] == id_voisin) {  // Si on trouve le voisin correspondant à id_voisin
            return i;    // Alors le port d’entrée est ce numéro (i)
        }
    }
    return UNKNOWN_INDEX;
}




size_t chercher_port_mac(Switch_s *sw, AdresseMac mac) {
    if (!sw) {
        return (size_t)-1;
    }

    for (size_t i = 0; i < sw->nb_ports; i++) {     // Parcourt tous les ports du switch
        if (sw->table_commutation[i].valide &&  // Si l'entrée est valide
            comparer_mac(sw->table_commutation[i].mac, mac)) {  // Et si l'adresse MAC correspond       // IL VA FALLOIR RETOURNER NUM PORT   
            return sw->table_commutation[i].port;   // On a trouvé la bonne entrée, retourne le port
        }
    }
    return (size_t)-1;  // Sinon, MAC inconnue
}




size_t voisin_sur_port(reseau *r, size_t id_switch, size_t port) {
    sommet voisins[32];     // Tableau pour stocker les voisins du switch
    size_t nb_voisins = sommets_adjacents(&r->graphe, id_switch, voisins);  // Récupère les voisins
    if (port < nb_voisins) {    // Si le port demandé existe
        return voisins[port];   // Retourne l'ID du voisin correspondant à ce port
    }
    return UNKNOWN_INDEX;
}



int port_utilise_par_table(Switch_s *sw, size_t port) {
    for (size_t j=0; j< sw->nb_asso; j++) {
        if (sw->table_commutation[j].valide && sw->table_commutation[j].port == port) {
            return 1;   // ce port est deja utilise
        }
    }
    return 0; // port libre
}









/*Retourne 0 si destination pas atteinte, 1 sinon*/
int envoyer_trame_rec(reseau *r, size_t id_actuel, TrameEthernet *trame, size_t id_precedent) {
    equipement *eq = &r->equipements[id_actuel];    // Récupère l'équipement courant
    char str_mac_src[18];
    char str_mac_dst[18];

    if (eq->type == EQUIPEMENT_SWITCH) {
        Switch_s *sw = &eq->equipement.sw;
        size_t port_entree = port_entre_de_precedent(r, id_actuel, id_precedent);

        // VÉRIFIER L'ÉTAT DU PORT D'ENTRÉE
        if (port_entree != UNKNOWN_INDEX && 
            port_entree < sw->nb_ports && 
            sw->etat_ports[port_entree] == PORT_BLOQUE) {
            printf("╚═ [Switch %zu] Trame bloquée - port %zu fermé\n", id_actuel, port_entree);
            return 0;  // Port bloqué, on n'accepte pas la trame
        }

        printf("╔═ [Switch %zu] Trame reçue via port %zu (de %zu)\n", id_actuel, port_entree, id_precedent);

        // Apprentissage (seulement si le port d'entrée est actif)
        if (port_entree != UNKNOWN_INDEX && port_entree < sw->nb_ports) {
            ajouter_commutation(sw, trame->adresse_source, port_entree);
            printf("║  Apprentissage : MAC %s → port %zu\n", mac_to_string(trame->adresse_source, str_mac_src), port_entree);
        }
        
        // Recherche destination
        size_t port_dest = chercher_port_mac(sw, trame->adresse_destination);
        mac_to_string(trame->adresse_destination, str_mac_dst);

        if (port_dest != (size_t)-1 && port_dest < sw->nb_ports) {
            // VÉRIFIER L'ÉTAT DU PORT DE SORTIE
            if (sw->etat_ports[port_dest] == PORT_BLOQUE) {
                printf("║  Port destination %zu bloqué, passage en diffusion\n", port_dest);
            } else {
                size_t voisin = voisin_sur_port(r, id_actuel, port_dest);
                printf("║  Destination connue : MAC %s → port %zu (vers %zu)\n", str_mac_dst, port_dest, voisin);
                
                if (voisin != UNKNOWN_INDEX && envoyer_trame_rec(r, voisin, trame, id_actuel)) {
                    return 1;
                }
            }
        }

        // Diffusion (seulement sur les ports actifs)
        printf("║  Destination inconnue : MAC %s → diffusion\n", str_mac_dst);
        sommet voisins[32];
        size_t nb_voisins = sommets_adjacents(&r->graphe, id_actuel, voisins);

        for (size_t i = 0; i < nb_voisins; i++) {
            if (voisins[i] == id_precedent) continue;  // Ne pas renvoyer par où c'est arrivé
            
            if (i < sw->nb_ports && sw->etat_ports[i] == PORT_BLOQUE) {
                printf("║    ↳ Port %zu → %zu (BLOQUÉ)\n", i, voisins[i]);
                continue;  // Port bloqué, passer au suivant
            }

            printf("║    ↳ Port %zu → %zu\n", i, voisins[i]);
            if (envoyer_trame_rec(r, voisins[i], trame, id_actuel)) {
                return 1;
            }
        }

        printf("╚═ [Switch %zu] Trame non transmise\n", id_actuel);
    }  else if (eq->type == EQUIPEMENT_STATION) {    // Si on atteint une station
        station *st = &eq->equipement.st;
        if (comparer_mac(st->mac, trame->adresse_destination)) {    // Si c’est la station destinataire
            printf("\033[1;32m🎯 [Station %zu] Trame reçue par la destination (%s)\033[0m\n", id_actuel, mac_to_string(trame->adresse_destination, str_mac_dst));    // Affiche que la trame est reçue
            return 1;
        }
        else {
            printf("⛔ [Station %zu] Trame ignorée (pas la bonne MAC)\n", id_actuel);
        }
    }
    return 0;   // pas trouvé
}

// Fonction pour vérifier si un port est actif
bool port_est_actif(Switch_s *sw, size_t port) {
    if (!sw || port >= sw->nb_ports) {
        return false;
    }
    return sw->etat_ports[port] != PORT_BLOQUE;
}

void envoyer_trame(reseau *r, size_t id_station_source, TrameEthernet *trame) {
    sommet voisins[32]; // Tableau pour stocker les voisins de la station source
    size_t nb_voisins = sommets_adjacents(&r->graphe, id_station_source, voisins);

    for (size_t i = 0; i < nb_voisins; i++) {   // Pour chaque voisin
        if (envoyer_trame_rec(r, voisins[i], trame, id_station_source)) { // Lance la propagation d la trame
            break;
        }
    }
}
