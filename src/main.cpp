#include "TSPParser.h"
#include "TSPInstance.h"
#include "Visualizer.h"
#include "monAlgo1.h"
#include "monAlgo2.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

static const char *INSTANCES_DIR = "instances/";

static const char *weight_type_name(int type) {
    switch (type) {
        case WEIGHT_TYPE_EXPLICIT: return "EXPLICIT (UPPER_ROW)";
        case WEIGHT_TYPE_EUC_2D:   return "EUC_2D";
        case WEIGHT_TYPE_ATT:      return "ATT";
        default:                   return "INCONNU";
    }
}

static void print_full_matrix(const TSPInstance *inst) {
    int n = inst->n;
    std::printf("  Matrice des distances (%dx%d) :\n\n", n, n);
    std::printf("      ");
    for (int j = 0; j < n; j++) std::printf("%7d", j);
    std::printf("\n      ");
    for (int j = 0; j < n; j++) std::printf("-------");
    std::printf("\n");
    for (int i = 0; i < n; i++) {
        std::printf("  %2d |", i);
        for (int j = 0; j < n; j++)
            std::printf("%7d", inst->dist[i][j]);
        std::printf("\n");
    }
}

static void print_instance_info(const TSPInstance *inst) {
    std::printf("  Nom       : %s\n", inst->name);
    std::printf("  Dimension : %d\n", inst->n);
    std::printf("  Type      : %s\n", weight_type_name(inst->weight_type));
    std::printf("\n");
    print_full_matrix(inst);
    if (inst->coords != nullptr) {
        int lim = (inst->n < 5) ? inst->n : 5;
        std::printf("\n  Coordonnees (5 premiers noeuds) :\n");
        for (int i = 0; i < lim; i++) {
            std::printf("    Noeud %2d : (%.1f, %.1f)\n",
                        i, inst->coords[i].x, inst->coords[i].y);
        }
    }
}

static void build_path(char *out, int out_size, const char *filename) {
    std::strncpy(out, INSTANCES_DIR, out_size - 1);
    out[out_size - 1] = '\0';
    std::strncat(out, filename, out_size - 1 - (int)std::strlen(out));
}

static bool test_file(const char *filename) {
    char path[512];
    build_path(path, sizeof(path), filename);

    TSPInstance inst;
    std::printf("Chargement de '%s' ...\n", filename);

    if (!tsp_load(path, &inst)) {
        std::printf("  [ECHEC] Impossible de charger '%s'.\n\n", filename);
        return false;
    }

    print_instance_info(&inst);

    // ── Algorithmes TSP ──────────────────────────────────────

    int n = inst.n;
    int* meilleurChemin = nullptr;
    int  meilleureDist  = -1;

    // Lancer Nearest Neighbor depuis chaque ville de départ
    for (int depart = 0; depart < n; depart++) {
        int* chemin = nearestNeighbor(&inst, depart);

        // Calculer la distance totale
        int dist = 0;
        for (int i = 0; i < n; i++)
            dist += inst.dist[chemin[i]][chemin[i + 1]];

        // Garder le meilleur chemin
        if (meilleureDist == -1 || dist < meilleureDist) {
            delete[] meilleurChemin;
            meilleurChemin = chemin;
            meilleureDist  = dist;
        } else {
            delete[] chemin;
        }
    }

    // Améliorer avec 2-opt
    twoOpt(&inst, meilleurChemin, n + 1);

    // Recalculer la distance après 2-opt
    int distFinale = 0;
    for (int i = 0; i < n; i++)
        distFinale += inst.dist[meilleurChemin[i]][meilleurChemin[i + 1]];

    // Afficher le résultat
    std::printf("\n  Meilleur chemin trouve :\n  ");
    for (int i = 0; i <= n; i++)
        std::printf("%d%s", meilleurChemin[i], i < n ? " -> " : "\n");
    std::printf("  Distance : %d\n\n", distFinale);

    delete[] meilleurChemin;

    // ────────────────────────────────────────────────────────

    visualize(&inst);
    tsp_free(&inst);
    std::printf("\n");
    return true;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::fprintf(stderr,
            "Usage : %s <fichier.tsp> [fichier2.tsp ...]\n\n"
            "Exemples :\n"
            "  %s bayg29.tsp\n"
            "  %s bayg29.tsp att48.tsp\n"
            "\nLes fichiers sont recherches dans le repertoire '%s'.\n",
            argv[0], argv[0], argv[0], INSTANCES_DIR);
        return EXIT_FAILURE;
    }

    // Redirection de stdout vers le fichier log
    std::freopen("logs/output.log", "w", stdout);

    std::printf("==============================================\n");
    std::printf("  Solveur TSP - Nearest Neighbor + 2-opt\n");
    std::printf("==============================================\n\n");

    int nb_ok    = 0;
    int nb_echec = 0;

    for (int i = 1; i < argc; i++) {
        std::printf("--- Fichier %d/%d ---\n", i, argc - 1);
        if (test_file(argv[i]))
            nb_ok++;
        else
            nb_echec++;
    }

    std::printf("==============================================\n");
    std::printf("  %d charge(s) avec succes, %d echec(s).\n", nb_ok, nb_echec);
    std::printf("==============================================\n");

    return (nb_echec == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}