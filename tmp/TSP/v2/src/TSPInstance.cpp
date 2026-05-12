#include "TSPInstance.h"
#include <cstring>
#include <cstdio>

bool tsp_alloc_matrix(TSPInstance *inst) {
    int n = inst->n;
    try {
        inst->dist = new int*[n];
        for (int i = 0; i < n; i++) {
            inst->dist[i] = new int[n];
            std::memset(inst->dist[i], 0, n * sizeof(int));
        }
    } catch (...) {
        std::fprintf(stderr, "[TSPInstance] Erreur d'allocation de la matrice.\n");
        inst->dist = nullptr;
        return false;
    }
    return true;
}

bool tsp_alloc_coords(TSPInstance *inst) {
    int n = inst->n;
    try {
        inst->coords = new NodeCoord[n];
        std::memset(inst->coords, 0, n * sizeof(NodeCoord));
    } catch (...) {
        std::fprintf(stderr, "[TSPInstance] Erreur d'allocation des coordonnees.\n");
        inst->coords = nullptr;
        return false;
    }
    return true;
}

void tsp_free(TSPInstance *inst) {
    if (inst->dist != nullptr) {
        for (int i = 0; i < inst->n; i++) {
            delete[] inst->dist[i];
        }
        delete[] inst->dist;
        inst->dist = nullptr;
    }
    if (inst->coords != nullptr) {
        delete[] inst->coords;
        inst->coords = nullptr;
    }
}

bool tsp_is_valid(const TSPInstance *inst) {
    if (inst->n <= 0) {
        std::fprintf(stderr, "[TSPInstance] Dimension invalide : %d\n", inst->n);
        return false;
    }
    if (inst->dist == nullptr) {
        std::fprintf(stderr, "[TSPInstance] Matrice de distances non allouee.\n");
        return false;
    }
    return true;
}
