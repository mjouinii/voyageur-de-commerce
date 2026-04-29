#include "../include/CoordDistanceCalc.h"
#include <cmath>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Lecture des coordonnées
// ---------------------------------------------------------------------------
// Le format NODE_COORD_SECTION est :
//   <numéro_nœud>  <x>  <y>
// Les numéros sont 1-based dans le fichier ; on les convertit en 0-based
// lors du stockage dans le vecteur coords (indice = numéro - 1).
//
// On lit exactement n lignes (une par nœud).
// ---------------------------------------------------------------------------

void CoordDistanceCalc::read(std::istream& in, TSPInstance& instance) {
    const int n = instance.n;
    instance.coords.resize(n);

    // --- Lecture des coordonnées ---
    for (int k = 0; k < n; ++k) {
        int nodeId;
        double x, y;
        if (!(in >> nodeId >> x >> y)) {
            throw std::runtime_error(
                "CoordDistanceCalc : coordonnées manquantes au nœud " + std::to_string(k + 1)
            );
        }
        // Conversion 1-based → 0-based
        int idx = nodeId - 1;
        if (idx < 0 || idx >= n) {
            throw std::runtime_error(
                "CoordDistanceCalc : numéro de nœud invalide " + std::to_string(nodeId)
            );
        }
        instance.coords[idx] = {x, y};
    }

    // --- Construction de la matrice de distances ---
    // On calcule les n*(n-1)/2 paires et on remplit les deux côtés
    // (symétrie) en même temps pour éviter de recalculer.
    // La diagonale reste à 0.
    instance.dist.assign(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int d = compute(instance.coords[i], instance.coords[j]);
            instance.dist[i][j] = d;
            instance.dist[j][i] = d;
        }
    }
}

// ---------------------------------------------------------------------------
// Formules de distance
// ---------------------------------------------------------------------------

int CoordDistanceCalc::compute(const NodeCoord& a, const NodeCoord& b) const {
    double dx = a.x - b.x;
    double dy = a.y - b.y;

    if (type_ == DistanceType::EUC_2D) {
        // Arrondi à l'entier le plus proche (nint = nearest int)
        // std::round() fait exactement ça.
        return static_cast<int>(std::round(std::sqrt(dx*dx + dy*dy)));
    }
    else { // ATT
        // Formule officielle TSPLIB section 2.3 :
        //   rij  = sqrt( (dx² + dy²) / 10 )
        //   tij  = nint(rij)
        //   dij  = tij < rij ? tij + 1 : tij
        //
        // Ce n'est PAS exactement ceil() car si rij = 3.0 exactement,
        // ceil(3.0) = 3 ET la formule donne aussi 3 → cohérent.
        // Mais ceil() sur un double avec erreur flottante pourrait donner
        // 4 alors que la valeur mathématique est 3. La formule avec nint
        // est plus stable numériquement.
        double rij = std::sqrt((dx*dx + dy*dy) / 10.0);
        int    tij = static_cast<int>(std::round(rij));
        return (tij < rij) ? tij + 1 : tij;
    }
}