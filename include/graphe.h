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

static const size_t UNKNOWN_INDEX = -1;

void init_graphe(graphe *g);    //    // initialise les champs internes du graphe g

void deinit_graphe(graphe *g);      // libère la mémoire qui avait été allouée dans la fonction init_graphe

size_t ordre(graphe const *g);    // retourne l'index du sommet s dans le graphe g
size_t nb_aretes(graphe const *g);

void ajouter_sommet(graphe *g);
size_t index_sommet(graphe const *g, sommet s);
bool existe_arete(graphe const *g, arete a);    // retourne true si l'arête a est contenue dans le graphe g, false sinon
bool ajouter_arete(graphe *g, arete a);     // l'arête a n'est ajoutée que si les conditions suivantes sont remplies :    retourne true si l'arête a bien été ajoutée, false sinon

size_t index_arete(graphe const *g, arete a);       // retourne l'index de l'arête au sein du tableau d'arêtes de g si l'arête a existe dans g,

size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[]);       // remplit le tableau sa avec les sommets adjacents de s dans g
    // et retourne le nombre de sommets ainsi stockés
