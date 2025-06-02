#pragma once

#include "graphe.h"
#include <stdint.h>

size_t degre(graphe *g, sommet s);
bool est_regulier(graphe *g);

void afficher(graphe *g);

void generer_complet(graphe *g, size_t ordre);

void visite_composante_connexe(graphe *g, sommet s, bool *visite);
uint32_t nb_composantes_connexes(graphe *g);

bool sont_connectes(graphe *g, sommet s1, sommet s2);

void coloriage_glouton(graphe *g, uint8_t *couleur_sommet);
void appliquer_permutation(graphe *src, graphe *dst, size_t *permutation);
uint32_t estimation_nb_chromatique(graphe *g, uint32_t n);
void generer_aleatoire(graphe *g, size_t ordre, uint32_t k);

void dijkstra(graphe *g, sommet s, double *poids_arete, double *distance_sommet);

void trier_aretes(arete *aretes_triees, graphe *g, double *poids_arete);
void kruskal(graphe *g, double *poids_arete, graphe *acm);
