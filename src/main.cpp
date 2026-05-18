#include "TSPParser.h"
#include "TSPInstance.h"
#include "Visualizer.h"
#include "monAlgo1.h"
#include "monAlgo2.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

static const char *INSTANCES_DIR = "instances/";

// Retourne le nom lisible du type de pondération
static const char *weight_type_name(int type) {
    switch (type) {
        case WEIGHT_TYPE_EXPLICIT: return "EXPLICIT (UPPER_ROW)";
        case WEIGHT_TYPE_EUC_2D:   return "EUC_2D";
        case WEIGHT_TYPE_ATT:      return "ATT";
        default:                   return "INCONNU";
    }
}

// Construit le chemin complet vers le fichier instance
static void build_path(char *out, int out_size, const char *filename) {
    std::strncpy(out, INSTANCES_DIR, out_size - 1);
    out[out_size - 1] = '\0';
    std::strncat(out, filename, out_size - 1 - static_cast<int>(std::strlen(out)));
}

// Charge, résout et affiche les résultats pour un fichier .tsp
static bool test_file(const char *filename) {

    char path[512];
    build_path(path, sizeof(path), filename);

    TSPInstance inst;
    std::printf("Chargement de '%s' ...\n", filename);

    if (!tsp_load(path, &inst)) {
        std::printf("  [ECHEC] Impossible de charger '%s'.\n\n", filename);
        return false;
    }

    // ── Informations de base ─────────────────────────────────────────────────
    std::printf("\n");
    std::printf("  Nom       : %s\n", inst.name);
    std::printf("  Dimension : %d villes\n", inst.n);
    std::printf("  Type      : %s\n", weight_type_name(inst.weight_type));
    std::printf("  Coords    : %s\n", inst.has_coords ? "oui" : "non");
    std::printf("\n");

    int  n             = inst.n;
    int *meilleurChemin = nullptr;
    int  meilleureDist  = -1;

    // ── Nearest Neighbor depuis chaque ville de départ ───────────────────────
    std::printf("  Recherche du meilleur chemin (Nearest Neighbor)...\n");

    for (int depart = 0; depart < n; depart++) {

        int *chemin = nearestNeighbor(&inst, depart);

        // Calcul de la distance totale du chemin
        int dist = 0;
        for (int i = 0; i < n; i++)
            dist += inst.dist[chemin[i]][chemin[i + 1]];

        // Conservation du meilleur chemin trouvé
        if (meilleureDist == -1 || dist < meilleureDist) {
            delete[] meilleurChemin;
            meilleurChemin = chemin;
            meilleureDist  = dist;
        } else {
            delete[] chemin;
        }
    }

    std::printf("  Nearest Neighbor : distance = %d\n", meilleureDist);

    // ── Amélioration par 2-opt ───────────────────────────────────────────────
    twoOpt(&inst, meilleurChemin, n + 1);

    int distFinale = 0;
    for (int i = 0; i < n; i++)
        distFinale += inst.dist[meilleurChemin[i]][meilleurChemin[i + 1]];

    std::printf("  Après 2-opt      : distance = %d\n", distFinale);
    std::printf("\n");

    // ── Affichage du chemin ──────────────────────────────────────────────────
    std::printf("  Meilleur chemin trouvé :\n  ");
    for (int i = 0; i <= n; i++) {
        std::printf("%d", meilleurChemin[i]);
        if (i < n) std::printf(" -> ");
    }
    std::printf("\n\n");
    std::printf("  Distance finale : %d\n\n", distFinale);

    // ── Visualisation (uniquement si des coordonnées sont disponibles) ───────
    if (inst.has_coords) {
        std::printf("  Ouverture de la fenêtre graphique...\n");
        visualize(&inst);
    } else {
        std::printf("  Pas de coordonnées : visualisation ignorée.\n");
    }

    delete[] meilleurChemin;
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
            "\nLes fichiers sont recherchés dans le répertoire '%s'.\n",
            argv[0], argv[0], argv[0], INSTANCES_DIR);
        return EXIT_FAILURE;
    }

    std::printf("==============================================\n");
    std::printf("  Solveur TSP - Nearest Neighbor + 2-opt\n");
    std::printf("==============================================\n\n");

    int nb_ok    = 0;
    int nb_echec = 0;

    for (int i = 1; i < argc; i++) {
        if (test_file(argv[i]))
            nb_ok++;
        else
            nb_echec++;
    }

    // ── Bilan final ──────────────────────────────────────────────────────────
    std::printf("==============================================\n");
    std::printf("  Bilan : %d succès, %d échec(s)\n", nb_ok, nb_echec);
    std::printf("==============================================\n");

    return (nb_echec == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
