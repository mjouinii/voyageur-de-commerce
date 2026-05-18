#include "ExplicitMatrixReader.h"
#include "TSPInstance.h"
#include <cstdio>
#include <cstring>  // pour std::strncpy, std::strncmp

// Déclaration anticipée car read_explicit_upper_row appelle read_display_data_section
bool read_display_data_section(FILE *file, TSPInstance *inst);

// Lit la matrice UPPER_ROW : triangle supérieur, reconstruit la matrice symétrique
bool read_explicit_upper_row(FILE *file, TSPInstance *inst) {
    int n = inst->n;

    // Diagonale à zéro
    for (int i = 0; i < n; i++)
        inst->dist[i][i] = 0;

    // Lecture du triangle supérieur et remplissage symétrique
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            int v;
            if (std::fscanf(file, "%d", &v) != 1) {
                std::fprintf(stderr,
                    "[ExplicitMatrixReader] Erreur : valeur manquante "
                    "à la position (%d, %d).\n", i, j);
                return false;
            }
            inst->dist[i][j] = v;
            inst->dist[j][i] = v;
        }
    }

    // Après la matrice, cherche DISPLAY_DATA_SECTION dans le reste du fichier
    char line[512];
    while (std::fgets(line, sizeof(line), file) != nullptr) {

        // Conversion en majuscules pour comparaison
        char upper[512];
        std::strncpy(upper, line, sizeof(upper) - 1);
        upper[sizeof(upper) - 1] = '\0';
        for (int i = 0; upper[i] != '\0'; i++) {
            if (upper[i] >= 'a' && upper[i] <= 'z')
                upper[i] = static_cast<char>(upper[i] - 'a' + 'A');
        }

        // Trim gauche
        const char *up = upper;
        while (*up == ' ' || *up == '\t' || *up == '\r' || *up == '\n')
            up++;

        if (std::strncmp(up, "DISPLAY_DATA_SECTION", 20) == 0) {
            // Lit les coordonnées d'affichage
            if (!read_display_data_section(file, inst))
                return false;
            break;
        }

        if (std::strncmp(up, "EOF", 3) == 0)
            break;
    }

    return true;
}

// Lit DISPLAY_DATA_SECTION : stocke les coordonnées pour la visualisation
bool read_display_data_section(FILE *file, TSPInstance *inst) {
    int n = inst->n;

    // Alloue les coordonnées si ce n'est pas déjà fait
    if (inst->coords == nullptr) {
        if (!tsp_alloc_coords(inst)) {
            std::fprintf(stderr,
                "[ExplicitMatrixReader] Erreur d'allocation des coordonnées.\n");
            return false;
        }
    }

    for (int k = 0; k < n; k++) {
        int    node_id;
        double x, y;

        if (std::fscanf(file, "%d %lf %lf", &node_id, &x, &y) != 3) {
            std::fprintf(stderr,
                "[ExplicitMatrixReader] Erreur : coordonnées manquantes "
                "au nœud %d dans DISPLAY_DATA_SECTION.\n", k + 1);
            return false;
        }

        if (node_id < 1 || node_id > n) {
            std::fprintf(stderr,
                "[ExplicitMatrixReader] Identifiant invalide %d "
                "(doit être entre 1 et %d).\n", node_id, n);
            return false;
        }

        inst->coords[node_id - 1].x = x;
        inst->coords[node_id - 1].y = y;
    }

    // Coordonnées disponibles pour la visualisation
    inst->has_coords = true;

    return true;
}
