#include "CoordDistanceCalc.h"
#include "TSPInstance.h"
#include <cstdio>
#include <cmath>

// Arrondi à l'entier le plus proche pour x >= 0
static int nint_positive(double x) {
    return static_cast<int>(x + 0.5);
}

// Distance euclidienne 2D arrondie (norme EUC_2D de TSPLIB)
int compute_distance_euc2d(const TSPInstance *inst, int i, int j) {
    double dx = inst->coords[i].x - inst->coords[j].x;
    double dy = inst->coords[i].y - inst->coords[j].y;
    return nint_positive(std::sqrt(dx * dx + dy * dy));
}

// Distance ATT (pseudo-euclidienne, norme ATT de TSPLIB)
int compute_distance_att(const TSPInstance *inst, int i, int j) {
    double dx = inst->coords[i].x - inst->coords[j].x;
    double dy = inst->coords[i].y - inst->coords[j].y;
    double r  = std::sqrt((dx * dx + dy * dy) / 10.0);
    int    t  = nint_positive(r);
    // Si t est inférieur à r, on prend t+1 pour garantir t >= r
    return (t < r) ? t + 1 : t;
}

// Lit NODE_COORD_SECTION et remplit inst->coords puis inst->dist[][]
bool read_node_coord_section(FILE *file, TSPInstance *inst) {
    int n = inst->n;

    // Allocation du tableau de coordonnées
    if (!tsp_alloc_coords(inst)) {
        std::fprintf(stderr,
            "[CoordDistanceCalc] Erreur d'allocation des coordonnées.\n");
        return false;
    }

    // Passe 1 : lecture des coordonnées
    for (int k = 0; k < n; k++) {
        int    node_id;
        double x, y;

        if (std::fscanf(file, "%d %lf %lf", &node_id, &x, &y) != 3) {
            std::fprintf(stderr,
                "[CoordDistanceCalc] Erreur : coordonnées manquantes "
                "au nœud %d (attendu %d nœuds).\n", k + 1, n);
            return false;
        }

        if (node_id < 1 || node_id > n) {
            std::fprintf(stderr,
                "[CoordDistanceCalc] Erreur : identifiant invalide %d "
                "(doit être entre 1 et %d).\n", node_id, n);
            return false;
        }

        inst->coords[node_id - 1].x = x;
        inst->coords[node_id - 1].y = y;
    }

    // Les coordonnées sont disponibles pour la visualisation
    inst->has_coords = true;

    // Passe 2 : calcul de la matrice de distances symétrique
    for (int i = 0; i < n; i++) {
        inst->dist[i][i] = 0;
        for (int j = i + 1; j < n; j++) {
            int d;
            if (inst->weight_type == WEIGHT_TYPE_ATT)
                d = compute_distance_att(inst, i, j);
            else
                d = compute_distance_euc2d(inst, i, j);

            inst->dist[i][j] = d;
            inst->dist[j][i] = d;
        }
    }

    return true;
}
