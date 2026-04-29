#pragma once
#include "TSPInstance.h"
#include <istream>

// ---------------------------------------------------------------------------
// CoordDistanceCalc
// ---------------------------------------------------------------------------
// Responsabilité : lire la section NODE_COORD_SECTION et calculer toutes
// les distances selon le type de métrique indiqué (ATT, EUC_2D…).
//
// Métriques supportées :
//
//   EUC_2D  – distance euclidienne arrondie à l'entier le plus proche.
//             d(i,j) = round( sqrt((xi-xj)² + (yi-yj)²) )
//
//   ATT     – "pseudo-euclidienne" utilisée dans att48.
//             Formule officielle TSPLIB :
//               rij = sqrt( ((xi-xj)² + (yi-yj)²) / 10 )
//               tij = nint(rij)          // arrondi normal
//               if (tij < rij) dij = tij + 1
//               else           dij = tij
//             Soit : dij = ceil(rij) si rij n'est pas entier, round sinon.
//             En pratique : dij = (int)ceil(sqrt(((xi-xj)²+(yi-yj)²)/10.0))
//             mais il faut éviter de sur-arrondir quand rij est exactement
//             entier → on utilise la formule avec nint explicite.
//
// Pourquoi séparer EUC_2D et ATT ?
//   La seule différence est la formule de calcul. En passant le type au
//   constructeur, on évite un if/switch dans la boucle interne (n² appels).
// ---------------------------------------------------------------------------

enum class DistanceType { EUC_2D, ATT };

class CoordDistanceCalc {
public:
    explicit CoordDistanceCalc(DistanceType type) : type_(type) {}

    // Lit NODE_COORD_SECTION (flux positionné après cette ligne),
    // stocke les coordonnées et calcule la matrice dist[][].
    void read(std::istream& in, TSPInstance& instance);

private:
    DistanceType type_;

    // Calcule la distance entière entre deux nœuds selon la métrique choisie
    int compute(const NodeCoord& a, const NodeCoord& b) const;
};