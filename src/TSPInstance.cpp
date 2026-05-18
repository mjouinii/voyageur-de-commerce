#include "TSPInstance.h"
#include <cstring>
#include <cstdio>
#include <new>      // pour std::nothrow

// Alloue la matrice de distances n x n et l'initialise à zéro
bool tsp_alloc_matrix(TSPInstance *inst) {
    int n = inst->n;

    inst->dist = new (std::nothrow) int*[n];
    if (inst->dist == nullptr) {
        std::fprintf(stderr, "[TSPInstance] Erreur d'allocation de la matrice.\n");
        return false;
    }

    for (int i = 0; i < n; i++) {
        inst->dist[i] = new (std::nothrow) int[n];
        if (inst->dist[i] == nullptr) {
            std::fprintf(stderr, "[TSPInstance] Erreur d'allocation ligne %d.\n", i);
            // Libère les lignes déjà allouées avant de retourner
            for (int k = 0; k < i; k++)
                delete[] inst->dist[k];
            delete[] inst->dist;
            inst->dist = nullptr;
            return false;
        }
        std::memset(inst->dist[i], 0, n * sizeof(int));
    }

    return true;
}

// Alloue le tableau de coordonnées et l'initialise à zéro
bool tsp_alloc_coords(TSPInstance *inst) {
    int n = inst->n;

    inst->coords = new (std::nothrow) NodeCoord[n];
    if (inst->coords == nullptr) {
        std::fprintf(stderr, "[TSPInstance] Erreur d'allocation des coordonnées.\n");
        return false;
    }

    std::memset(inst->coords, 0, n * sizeof(NodeCoord));
    return true;
}

// Libère toute la mémoire allouée dans l'instance
void tsp_free(TSPInstance *inst) {
    if (inst->dist != nullptr) {
        for (int i = 0; i < inst->n; i++)
            delete[] inst->dist[i];
        delete[] inst->dist;
        inst->dist = nullptr;
    }

    if (inst->coords != nullptr) {
        delete[] inst->coords;
        inst->coords = nullptr;
    }

    inst->has_coords = false;
}

// Vérifie que l'instance est correctement initialisée
bool tsp_is_valid(const TSPInstance *inst) {
    if (inst->n <= 0) {
        std::fprintf(stderr, "[TSPInstance] Dimension invalide : %d\n", inst->n);
        return false;
    }
    if (inst->dist == nullptr) {
        std::fprintf(stderr, "[TSPInstance] Matrice de distances non allouée.\n");
        return false;
    }
    return true;
}
