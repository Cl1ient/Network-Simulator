#include "../include/reseau.h"
#include "../include/graphe.h"
#include "../include/algos.h"
#include "../include/tram.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

reseau creer_reseau() {
    reseau r;
    r.nb_equipements = 0;
    r.nb_equipements = 0;


    init_graphe(&r.graphe);

    return r;
}

// Connexion de deux équipements (ajt d’une arete dans le graphe)
bool connecter_equipement(reseau *reseau, size_t id1, size_t id2, int poids) {
    if (!reseau) {
        return false;
    }

    arete nouvelle_arete = {id1, id2, poids};

    return ajouter_arete(&reseau->graphe, nouvelle_arete);
}



void afficher_reseau(reseau *reseau) {
    if (!reseau) {
        printf("Réseau invalide\n");
        return;
    }
    printf("Réseau\n");
    printf("Équipements : %d\n", reseau->nb_equipements);
    for (int i = 0; i < reseau->nb_equipements; i++) {
        if (reseau->equipements[i].type == EQUIPEMENT_STATION)
            afficher_station(&reseau->equipements[i].equipement.st);
        else if (reseau->equipements[i].type == EQUIPEMENT_SWITCH)
            afficher_switch(&reseau->equipements[i].equipement.sw);
    }
    printf("\nStructure du graphe :\n");
    afficher(&reseau->graphe);
}

/*
// Fonction récursive d'envoi
void envoyer_trame_recursif(reseau *r, size_t courant, size_t id_dst, TrameEthernet *trame, int *visites) {

    if (visites[courant]) return;
    visites[courant] = 1;

    // Si on est sur la station de destination
    if (courant == id_dst) {
        printf("\nTrame reçue par la station destination [%zu] :\n", id_dst);
        afficherTrameUser(trame, 46); // ou longueur réelle
        return;
    }

    // Si on est sur un switch, on peut apprendre la MAC source (optionnel ici)
    if (r->equipements[courant].type == EQUIPEMENT_SWITCH) {
        printf("→ Passage par Switch %zu : ", courant);
        afficherMac(r->equipements[courant].equipement.sw.adresseMac);
        // Optionnel : apprentissage sur le premier port libre
        ajouter_commutation(&r->equipements[courant].equipement.sw, trame->adresse_source, 0);
    }

    // Flood sur tous les voisins non visités (pas de gestion de port d'arrivée nécessaire sans cycle)
    for (size_t i = 0; i < r->graphe.nb_aretes; i++) {
        arete a = r->graphe.aretes[i];
        size_t voisin = SIZE_MAX;
        if (a.s1 == courant) voisin = a.s2;
        else if (a.s2 == courant) voisin = a.s1;
        else continue;

        if (!visites[voisin]) {
            envoyer_trame_recursif(r, voisin, id_dst, trame, visites);
        }
    }
}

void envoyer_trame_via_tous_les_switchs(reseau *r, size_t id_src, size_t id_dst, uint8_t *donnees, size_t longueur) {
    if (!r || id_src >= r->nb_equipements || id_dst >= r->nb_equipements) {
        printf("ID invalide pour les équipements.\n");
        return;
    }

    TrameEthernet trame;
    memset(&trame, 0, sizeof(TrameEthernet));
    memset(trame.preambule, 0xAA, 7);
    trame.sfd = 0xAB;

    trame.adresse_source = r->equipements[id_src].equipement.st.mac;

    trame.adresse_destination = r->equipements[id_dst].equipement.st.mac;

    trame.type[0] = 0x08;
    trame.type[1] = 0x00;
    memcpy(trame.data, donnees, longueur);
    memset(trame.fcs, 0xDE, 4);

    printf("\n--- Début de l'envoi ---\n");
    printf("Source [%zu] => Destination [%zu]\n", id_src, id_dst);

    int *visites = calloc(r->nb_equipements, sizeof(int));
    // Initialiser tous les éléments à 1
    for (int i = 0; i < r->nb_equipements; i++) {
        visites[i] = 0;
    }

    // initialiser a 1 pour tout
    envoyer_trame_recursif(r, id_src, id_dst, &trame, visites);
    free(visites);
}
*/





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
    for (size_t i = 0; i < sw->nb_ports; i++) {     // Parcourt tous les ports du switch
        if (sw->table_commutation[i].valide &&  // Si l'entrée est valide
            comparer_mac(sw->table_commutation[i].mac, mac)) {  // Et si l'adresse MAC correspond   
            return i;   // On a trouvé la bonne entrée, retourne le port
        }
    }
    return -1;  // Sinon, MAC inconnue
}




size_t voisin_sur_port(reseau *r, size_t id_switch, size_t port) {
    sommet voisins[32];     // Tableau pour stocker les voisins du switch
    size_t nb_voisins = sommets_adjacents(&r->graphe, id_switch, voisins);  // Récupère les voisins
    if (port < nb_voisins) {    // Si le port demandé existe
        return voisins[port];   // Retourne l'ID du voisin correspondant à ce port
    }
    return UNKNOWN_INDEX;
}









/*Retourne 0 si destination pas atteinte, 1 sinon*/
int envoyer_trame_rec(reseau *r, size_t id_actuel, TrameEthernet *trame, size_t id_precedent) {
    equipement *eq = &r->equipements[id_actuel];    // Récupère l'équipement courant
    char str_mac_src[18];
    char str_mac_dst[18];

    if (eq->type == EQUIPEMENT_SWITCH) {    // Si c'est un switch
        Switch_s *sw = &eq->equipement.sw;  // Récupère le switch
        size_t port_entree = port_entre_de_precedent(r, id_actuel, id_precedent);    // Trouve le port par lequel la trame est arrivée

        printf("[Switch %zu] Trame reçue via port %zu (venant de %zu)\n", id_actuel, port_entree, id_precedent);

        //  ajoute l'adresse source à la table du switch
        ajouter_commutation(sw, trame->adresse_source, port_entree);
        printf("[Switch %zu] Apprentissage MAC source %s → port %zu\n", id_actuel, mac_to_string(trame->adresse_source, str_mac_src), port_entree);
        // Recherche si le switch connaît la MAC destination
        size_t port_dest = chercher_port_mac(sw, trame->adresse_destination);

        if (port_dest != -1) {
            size_t voisin = voisin_sur_port(r, id_actuel, port_dest);   // Récupère le voisin connecté à ce port
            printf("[Switch %zu] MAC destination %s connue → envoi vers port %zu (vers %zu)\n", id_actuel, mac_to_string(trame->adresse_destination, str_mac_dst), port_dest, voisin);            
            if (envoyer_trame_rec(r, voisin, trame, id_actuel)) {   // Envoie la trame à ce voisin
                return 1;
            }
        } else {
            printf("[Switch %zu] MAC destination %s inconnue → diffusion vers autres ports\n", id_actuel, mac_to_string(trame->adresse_destination, str_mac_dst));             // Si la MAC destination est inconnue → diffusion
            sommet voisins[32];
            size_t nb_voisins = sommets_adjacents(&r->graphe, id_actuel, voisins);  // Récupère tous les voisins

            for (size_t i = 0; i < nb_voisins; i++) {    // Parcourt tous les ports/voisins
                if (i == port_entree) continue; // Ne pas renvoyer la trame d’où elle vient

                if (!sw->table_commutation[i].valide) { // N'envoie que vers les ports "vides" dont on connait pas encore la mac
                    printf("  [Switch %zu] → Diffusion vers port %zu (vers %zu)\n", id_actuel, i, voisins[i]);
                    if (envoyer_trame_rec(r, voisins[i], trame, id_actuel)) { // Appel récursif vers le voisin
                        return 1;
                    }
                }
            }
        }

    } else if (eq->type == EQUIPEMENT_STATION) {    // Si on atteint une station
        station *st = &eq->equipement.st;
        if (comparer_mac(st->mac, trame->adresse_destination)) {    // Si c’est la station destinataire
            printf(">>> Trame reçue par la station destination !\n");    // Affiche que la trame est reçue
        }
        else {
            printf("[Station %zu] Trame ignorée (MAC ne correspond pas)\n", id_actuel);
        }
    }
    return 0;   // pas trouvé
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
