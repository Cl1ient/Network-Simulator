#include "../include/algos.h"
#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

size_t degre(graphe const *g, sommet s)
{
    sommet sa[g->ordre];        // tableau pour stocker les sommets adjacents
    return sommets_adjacents(g,s,sa);       // recuperer le nombre d'adjacent
}

bool est_regulier(graphe const *g)
{
    if(g->ordre == 0) {
        return true;
    }

    size_t deg = degre(g,0);        // degre du premier sommet
    for (size_t i = 1; i < ordre(g); i++) {     // on compare les autres sommets au premier
        if (degre(g,i) != deg ) {
            return false;
        }
    }
    return true;
}

void afficher(graphe const *g)
{
    printf("# sommets = %zu\n", ordre(g));
    printf("# arêtes = %zu\n", nb_aretes(g));

    printf("--SOMMETS--\n");
    for (size_t i = 0; i < ordre(g); i++) {
        sommet sa[g->ordre];
        size_t nb_adj = sommets_adjacents(g, i, sa);

        printf("%zu (degré: %zu) <-> ", i, degre(g, i));
        for (size_t j = 0; j < nb_adj; j++) {
            printf("%zu ", sa[j]);
        }
        printf("\n");
    }

    printf("--ARÊTES--\n");
    for (size_t i = 0; i < nb_aretes(g); i++) {         // pour tout les aretes du graphe, on affiche les aretes
        printf("%zu - %zu\n", g->aretes[i].s1, g->aretes[i].s2);
    }

}

void generer_complet(graphe *g, size_t ordre)
{

    for (size_t i = 0; i < ordre; i++) {
        ajouter_sommet(g);
    }

    for (size_t i = 0; i < ordre; i++) {
        for (size_t j = i + 1; j < ordre; j++) {
            ajouter_arete(g, (arete){i, j});
        }
    }


}

void visite_composante_connexe(graphe const *g, sommet s, bool *visite)
{
    visite[s] = true;

    sommet adj[g->ordre];  // tableau temp pour stocker les voisins
    size_t nb_adj = sommets_adjacents(g, s, adj);

    for (size_t i = 0; i < nb_adj; i++) {
        sommet voisin = adj[i];
        if (!visite[voisin]) {
            visite_composante_connexe(g, voisin, visite);
        }
    }
}

uint32_t nb_composantes_connexes(graphe const *g)
{
    size_t n = g->ordre;
    bool *visite = malloc(n * sizeof(bool));
    for (size_t i = 0; i < n; i++) {
        visite[i] = false;
    }
    uint32_t compteur = 0;

    for (size_t i = 0; i < n; i++) {
        if (!visite[i]) {
            visite_composante_connexe(g, i, visite);
            compteur++;
        }
    }

    free(visite);
    return compteur;

}

bool sont_connectes(graphe const *g, sommet s1, sommet s2)
{
    if (s1 == s2) {
        return true;
    }

    bool *visite = malloc(g->ordre * sizeof(bool));
    if (!visite) {
        return false;
    }

    for (size_t i = 0; i < g->ordre; i++) {
        visite[i] = false;
    }

    visite_composante_connexe(g, s1, visite);

    bool resultat = visite[s2];
    free(visite);
    return resultat;

}

void coloriage_glouton(graphe const *g, uint8_t *couleur_sommet)
{
    size_t n = ordre(g);        // nb de sommets
    for (size_t i = 0; i<n; i++) {
        couleur_sommet[i] = 255;    //non colorié
    }

    uint8_t couleur_max = 0;

    for (sommet s = 0; s<n; s++) {
        size_t deg = degre(g,s);    //nb de voisins
        sommet voisins[deg];    //pour stocker les voisins
        sommets_adjacents(g,s,voisins);

        // quels couleurs sont utilisés par les voisins
        uint8_t couleur_utilisee[couleur_max + 2];
        for (size_t i = 0; i<couleur_max + 2; i++) {
            couleur_utilisee[i] = 0;
        }

        for (size_t i = 0; i<deg; i++) {
            uint8_t c = couleur_sommet[voisins[i]];
            if (c != 255 && c <= couleur_max + 1) {
                couleur_utilisee[c] = 1;
            }
        }

        // plus petite coul non utilisée
        uint8_t couleur = 0;
        while (couleur_utilisee[couleur]) {
            couleur ++;
        }

        couleur_sommet[s] = couleur;
        if (couleur > couleur_max) {
            couleur_max = couleur;
        }
    }
}


void appliquer_permutation(graphe const *src, graphe *dst, size_t const *permutation)
{
    size_t n = ordre(src);

    for (size_t i = 0; i<n; i++) {
        ajouter_sommet(dst);
    }

    for (size_t i = 0; i<nb_aretes(src); i++) {
        arete a = src->aretes[i];

        sommet s1 = permutation[a.s1];
        sommet s2 = permutation[a.s2];

        ajouter_arete(dst, (arete){s1,s2});
    }
}

uint32_t estimation_nb_chromatique(graphe const *g, uint32_t n)
{
    size_t ordre_g = ordre(g);
    size_t *permutation = malloc(ordre_g * sizeof(size_t));
    uint8_t *couleurs = malloc(ordre_g * sizeof(uint8_t));
    uint32_t min_couleurs = ordre_g;        // 1 sommet = 1 couleur au début

    for (uint32_t i = 0; i<n; i++) {
        generer_permutation(permutation, ordre_g);

        graphe g_permute;
        init_graphe(&g_permute);
        appliquer_permutation(g, &g_permute, permutation);

        coloriage_glouton(&g_permute, couleurs);

        uint8_t max_couleur = 0;
        for (size_t j = 0; j<ordre_g; j++) {
            if (couleurs[j] != 255 && couleurs[j] > max_couleur) {
                max_couleur = couleurs[j];
            }
        }
        uint32_t nb_couleurs = max_couleur + 1;

        if (nb_couleurs < min_couleurs) {
            min_couleurs = nb_couleurs;
        }

        deinit_graphe(&g_permute);
    }

    free(permutation);
    free(couleurs);
    return min_couleurs;
}


void generer_aleatoire(graphe *g, size_t ordre, uint32_t k)
{
    for (size_t i = 0; i<ordre; i++) {
        ajouter_sommet(g);
    }

    for (size_t i = 0; i<ordre; i++) {
        for (size_t j = i+1; j<ordre; j++) {
            if (rand() % k == 0) {
                arete a = { .s1 = i, .s2 = j };
                ajouter_arete(g, a);
            }
        }
    }
}



void dijkstra(graphe const *g, sommet s, double const *poids_arete, double *distance_sommet)
{
    size_t n = ordre(g);
    
    // init distances
    for (size_t i = 0; i < n; i++) {
        distance_sommet[i] = DBL_MAX;
    }
    distance_sommet[s] = 0.0;
    
    // tab pour marquer les sommets visités
    bool *visite = malloc(n * sizeof(bool));
    for (size_t i = 0; i < n; i++) {
        visite[i] = false;
    }
    
    for (size_t compteur = 0; compteur < n; compteur++) {
        // sommet non visité avc la plus petite distance
        sommet u = 0;
        double min_distance = DBL_MAX;
        bool sommet_trouve = false;
        
        for (size_t i = 0; i < n; i++) {
            if (!visite[i] && distance_sommet[i] < min_distance) {
                min_distance = distance_sommet[i];
                u = i;
                sommet_trouve = true;
            }
        }
        
        // Si aucun sommet accessible 
        if (!sommet_trouve || distance_sommet[u] == DBL_MAX) {
            break;
        }
        
        visite[u] = true;
        
        // voisins de u
        sommet voisins[n];
        size_t nb_voisins = sommets_adjacents(g, u, voisins);
        
        // Pour chaque voisin
        for (size_t i = 0; i < nb_voisins; i++) {
            sommet v = voisins[i];
            
            if (!visite[v]) {
                arete a = {u, v};
                size_t index = index_arete(g, a);
                
                if (index != UNKNOWN_INDEX) {
                    double nouvelle_distance = distance_sommet[u] + poids_arete[index];
                    
                    if (nouvelle_distance < distance_sommet[v]) {
                        distance_sommet[v] = nouvelle_distance;
                    }
                }
            }
        }
    }
    
    free(visite);

}

void trier_aretes(arete *aretes_triees, graphe const *g, double const *poids_arete)
{
}

void kruskal(graphe const *g, double const *poids_arete, graphe *acm)
{
}

