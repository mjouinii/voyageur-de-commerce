#include "ExplicitMatrixReader.h"
#include <cstdio>

bool read_explicit_upper_row(FILE *file, TSPInstance *inst) {
    int n = inst->n;

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            int v;
            if (std::fscanf(file, "%d", &v) != 1) {
                std::fprintf(stderr,
                    "[ExplicitMatrixReader] Erreur : valeur manquante "
                    "a la position (%d, %d).\n", i, j);
                return false;
            }
            inst->dist[i][j] = v;
            inst->dist[j][i] = v;
        }
    }
    return true;
}
