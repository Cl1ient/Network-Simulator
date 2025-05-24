#include "../include/utils.h"

void generer_permutation(size_t *permutation, size_t n)
{
    int present[n];    // tab pour verif si un nb est deja pris

    for (size_t i = 0; i<n; i++) {
        present[i] = 0;
    }

    for (size_t i = 0; i<n; i++) {
        size_t val;

        // trouver une val non utilisé
        do {
            val = rand() % n;
        }
        while (present[val]);

        permutation[i] = val;
        present[val] = 1;
    }
}

void init_aleatoire(double *val, size_t n, double max)
{
}
