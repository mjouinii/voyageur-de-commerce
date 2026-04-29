#include "../include/TSPParser.h"
#include "../include/TSPVisualizer.h"
#include <iostream>
#include <iomanip>

// ---------------------------------------------------------------------------
// Programme de test : charge les deux fichiers et affiche un extrait de la
// matrice de distances pour vérifier la cohérence.
//
// Valeurs attendues (sources officielles TSPLIB) :
//   bayg29 : dist[0][1] = 97   (première valeur de UPPER_ROW)
//            dist[1][0] = 97   (symétrie)
//            dist[0][0] = 0    (diagonale)
//
//   att48  : dist[0][1] = ?    (calculé depuis les coordonnées)
//     Node 1 : (6734, 1453)
//     Node 2 : (2233, 10)
//     dx=4501, dy=1443 → sqrt((4501²+1443²)/10) = sqrt(2243.21) ≈ 47.36
//     nint(47.36)=47, 47 < 47.36 → d = 48
//     Donc dist[0][1] = 48  (valeur de référence TSPLIB connue : 48 ✓)
// ---------------------------------------------------------------------------

void printMatrixCorner(const TSPInstance& inst, int size = 5) {
    std::cout << "  Coin " << size << "x" << size << " de la matrice :\n";
    std::cout << "     ";
    for (int j = 0; j < size; ++j)
        std::cout << std::setw(6) << j;
    std::cout << "\n";
    for (int i = 0; i < size; ++i) {
        std::cout << "  " << std::setw(2) << i << " :";
        for (int j = 0; j < size; ++j)
            std::cout << std::setw(6) << inst.dist[i][j];
        std::cout << "\n";
    }
}

int main() {
    // -----------------------------------------------------------------------
    // Test 1 : bayg29  (EXPLICIT / UPPER_ROW)
    // -----------------------------------------------------------------------
    std::cout << "=== bayg29.tsp ===\n";
    try {
        TSPInstance b = TSPParser::load("instances/bayg29.tsp");
        std::cout << "  Nom       : " << b.name << "\n";
        std::cout << "  Dimension : " << b.n << " nœuds\n";
        std::cout << "  Valide    : " << (b.isValid() ? "oui" : "non") << "\n";
        std::cout << "  dist[0][1] = " << b.dist[0][1]
                  << "  (attendu : 97)\n";
        std::cout << "  dist[1][0] = " << b.dist[1][0]
                  << "  (symétrie)\n";
        std::cout << "  dist[0][0] = " << b.dist[0][0]
                  << "  (diagonale)\n";
        printMatrixCorner(b);
        
        // Appel du visualiseur
        std::cout << "  Ouverture de la fenêtre de visualisation pour bayg29...\n";
        TSPVisualizer::display(b);

    } catch (const std::exception& e) {
        std::cerr << "  ERREUR : " << e.what() << "\n";
    }

    std::cout << "\n";

    // -----------------------------------------------------------------------
    // Test 2 : att48  (ATT / NODE_COORD_SECTION)
    // -----------------------------------------------------------------------
    std::cout << "=== att48.tsp ===\n";
    try {
        TSPInstance a = TSPParser::load("instances/att48.tsp");
        std::cout << "  Nom       : " << a.name << "\n";
        std::cout << "  Dimension : " << a.n << " nœuds\n";
        std::cout << "  Valide    : " << (a.isValid() ? "oui" : "non") << "\n";
        std::cout << "  Coordonnées node 0 : ("
                  << a.coords[0].x << ", " << a.coords[0].y << ")\n";
        std::cout << "  Coordonnées node 1 : ("
                  << a.coords[1].x << ", " << a.coords[1].y << ")\n";
        std::cout << "  dist[0][1] = " << a.dist[0][1]
                  << "  (attendu : 48)\n";
        std::cout << "  dist[1][0] = " << a.dist[1][0]
                  << "  (symétrie)\n";
        printMatrixCorner(a);

        // Appel du visualiseur
        std::cout << "  Ouverture de la fenêtre de visualisation pour att48...\n";
        TSPVisualizer::display(a);

    } catch (const std::exception& e) {
        std::cerr << "  ERREUR : " << e.what() << "\n";
    }

    return 0;
}