#include "../include/graphe.h"

void init_graphe(graphe *g)
{
    // initialise les champs internes du graphe g
    // - allocation d'un tableau d'arêtes de capacité initiale 8
    // - le graphe ne contient initialement ni sommet ni arête

    g->ordre = 0;
    g->aretes_capacite = 8;
    g->nb_aretes = 0;
    g->aretes = malloc(g->aretes_capacite * sizeof(arete));
}

void deinit_graphe(graphe *g)
{
    // libère la mémoire qui avait été allouée dans la fonction init_graphe
    // réinitialise les champs internes du graphe g

    free(g->aretes);

    g->aretes = NULL;
    g->ordre = 0;
    g->aretes_capacite=0;
    g->nb_aretes=0;
}

size_t ordre(graphe *g)
{
    return g->ordre;
}

size_t nb_aretes(graphe *g)
{
    return g->nb_aretes;
}

void ajouter_sommet(graphe *g)
{
    g->ordre++;
}

size_t index_sommet(graphe *g, sommet s)
{
    // retourne l'index du sommet s dans le graphe g
    // la valeur UNKNOWN_INDEX si le sommet n'existe pas dans g

    if (s< g->ordre) {
        return s;
    }
    return UNKNOWN_INDEX;
}

// une fonction locale "static arete swap_sommets(arete a)" pourra être utile
// cette fonction retourne une nouvelle arête dont les sommets sont les même que l'arête reçue mais inversés

static arete swap_sommets(arete a)
{
    arete nv_arete;
    nv_arete.s1 = a.s2;
    nv_arete.s2 = a.s1;
    return nv_arete;
}


bool existe_arete(graphe *g, arete a)
{
    // retourne true si l'arête a est contenue dans le graphe g, false sinon
    // /!\ l'arête (s1,s2) et l'arête (s2,s1) sont considérées équivalentes

    for (size_t i = 0; i < g->nb_aretes; i++)
    {
        if (g->aretes[i].s1 == a.s1 && g->aretes[i].s2 == a.s2)
        {
            return true;
        }

        if (g->aretes[i].s1 == swap_sommets(a).s1 && g->aretes[i].s2 == swap_sommets(a).s2)
        {
            return true;
        }
    }

    return false;
}

bool ajouter_arete(graphe *g, arete a)
{
    // l'arête a n'est ajoutée que si les conditions suivantes sont remplies :
    //  - les sommets s1 et s2 de a existent dans g
    //  - les sommets s1 et s2 de a sont distincts
    //  - l'arête a n'existe pas dans g

    // /!\ si la capacité actuelle du tableau d'arêtes n'est pas suffisante,
    // /!\ il faut le réallouer.
    // /!\ on peut par exemple doubler la capacité du tableau actuel.

    // retourne true si l'arête a bien été ajoutée, false sinon

    if (index_sommet(g, a.s1) == UNKNOWN_INDEX || index_sommet(g, a.s2) == UNKNOWN_INDEX) {
        return false;
    }

    if (existe_arete(g, a)) {
        return false;
    }

    if (a.s1 == a.s2) {
        return false;
    }

    if (g->nb_aretes >= g->aretes_capacite){
        g->aretes_capacite *= 2;
        g->aretes = realloc(g->aretes, g->aretes_capacite * sizeof(arete));
    }

    g->aretes[g->nb_aretes] = a;
    g->aretes[g->nb_aretes].poids = a.poids;        // ajout de l'arete

    g-> nb_aretes ++;

    return true;
}

size_t index_arete(graphe *g, arete a)
{
    // retourne l'index de l'arête au sein du tableau d'arêtes de g si l'arête a existe dans g,
    // la valeur UNKNOWN_INDEX sinon

    arete a_inverse = swap_sommets(a);

    for (size_t i = 0; i < g->nb_aretes; i++) {
        if (g->aretes[i].s1 == a.s1 && g->aretes[i].s2 == a.s2) {
            return i;
        }
        if (g->aretes[i].s1 == a_inverse.s1 && g->aretes[i].s2 == a_inverse.s2) {
            return i;
        }
    }

    return UNKNOWN_INDEX;
}

size_t sommets_adjacents(graphe *g, sommet s, sommet sa[])
{
    // remplit le tableau sa avec les sommets adjacents de s dans g
    // et retourne le nombre de sommets ainsi stockés
    // (on suppose que s fait bien partie du graphe g)
    // (le tableau sa est supposé assez grand pour contenir les sommets adjacents de s)

    size_t count = 0;

    for (size_t i = 0; i < g->nb_aretes; i++) {
        if (g->aretes[i].s1 == s) {
            sa[count] = g->aretes[i].s2;
            count++;
        }
        else if (g->aretes[i].s2 == s) {
            sa[count] = g->aretes[i].s1;
            count++;
        }
    }

    return count;
}
