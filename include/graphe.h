#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef size_t sommet;

typedef struct arete
{
    sommet s1;
    sommet s2;
    int poids;      // pour gerer le poid d'un lien dans le reseau
} arete;

typedef struct graphe
{
    size_t ordre;   //nb de sommets
    arete *aretes;  // tab d'arêtes
    size_t aretes_capacite;     // capacité alloué (nb d'arêtes possible)
    size_t nb_aretes;   // nb réel d'arêtes dans le graphe
} graphe;

static size_t UNKNOWN_INDEX = -1;

void init_graphe(graphe *g);
void deinit_graphe(graphe *g);

size_t ordre(graphe *g);
size_t nb_aretes(graphe *g);

void ajouter_sommet(graphe *g);
size_t index_sommet(graphe *g, sommet s);
bool existe_arete(graphe *g, arete a);
bool ajouter_arete(graphe *g, arete a);
size_t index_arete(graphe *g, arete a);

size_t sommets_adjacents(graphe *g, sommet s, sommet sa[]);