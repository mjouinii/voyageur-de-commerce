#include "CoordDistanceCalc.h"
#include <cstdio>
#include <cmath>

/* Arrondi a l'entier le plus proche pour x >= 0 */
static int nint_positive(double x) {
    return static_cast<int>(x + 0.5);
}

int compute_distance_euc2d(const TSPInstance *inst, int i, int j) {
    double dx = inst->coords[i].x - inst->coords[j].x;
    double dy = inst->coords[i].y - inst->coords[j].y;
    return nint_positive(std::sqrt(dx * dx + dy * dy));
}

int compute_distance_att(const TSPInstance *inst, int i, int j) {
    double dx = inst->coords[i].x - inst->coords[j].x;
    double dy = inst->coords[i].y - inst->coords[j].y;
    double r  = std::sqrt((dx * dx + dy * dy) / 10.0);
    int    t  = nint_positive(r);
    return (t < r) ? t + 1 : t;
}

bool read_node_coord_section(FILE *file, TSPInstance *inst) {
    int n = inst->n;

    /* Passe 1 : lecture des coordonnees */
    for (int k = 0; k < n; k++) {
        int    node_id;
        double x, y;

        if (std::fscanf(file, "%d %lf %lf", &node_id, &x, &y) != 3) {
            std::fprintf(stderr,
                "[CoordDistanceCalc] Erreur : coordonnees manquantes "
                "au noeud %d (attendu %d noeuds).\n", k + 1, n);
            return false;
        }

        if (node_id < 1 || node_id > n) {
            std::fprintf(stderr,
                "[CoordDistanceCalc] Erreur : identifiant invalide %d "
                "(doit etre entre 1 et %d).\n", node_id, n);
            return false;
        }

        inst->coords[node_id - 1].x = x;
        inst->coords[node_id - 1].y = y;
    }

    /* Passe 2 : calcul de la matrice de distances */
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int d;
            if (inst->weight_type == WEIGHT_TYPE_ATT) {
                d = compute_distance_att(inst, i, j);
            } else {
                d = compute_distance_euc2d(inst, i, j);
            }
            inst->dist[i][j] = d;
            inst->dist[j][i] = d;
        }
    }

    return true;
}
