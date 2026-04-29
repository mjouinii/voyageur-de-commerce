#pragma once
#include <vector>
#include <string>

// ---------------------------------------------------------------------------
// TSPInstance
// ---------------------------------------------------------------------------
// Structure centrale qui représente une instance TSP entièrement chargée.
//
// Choix de conception :
//   - On stocke la matrice de distances sous forme d'un vector<vector<int>>.
//     Un tableau 2D plat (vector<int> de taille n*n) serait légèrement plus
//     rapide en cache, mais vector<vector<int>> est plus lisible et largement
//     suffisant pour n ≤ quelques milliers.
//   - Les distances sont des int (entiers) : le format TSPLIB garantit que
//     les distances finales sont entières (ou arrondies). Stocker des double
//     n'apporterait rien et consommerait deux fois plus de mémoire.
//   - Les coordonnées (optionnelles) sont en double car elles servent
//     uniquement à l'affichage ou à des heuristiques géométriques.
// ---------------------------------------------------------------------------

struct NodeCoord {
    double x, y;
};

class TSPInstance {
public:
    std::string name;                       // champ NAME du fichier
    int         n = 0;                      // nombre de nœuds (DIMENSION)
    std::vector<std::vector<int>> dist;     // dist[i][j] = distance i→j (0-indexé)
    std::vector<NodeCoord>        coords;   // coordonnées (vides si EXPLICIT)

    // Accès sûr à la distance entre deux nœuds (indices 0-based)
    int distance(int i, int j) const { return dist[i][j]; }

    // Retourne true si l'instance est complètement chargée
    bool isValid() const { return n > 0 && (int)dist.size() == n; }
};