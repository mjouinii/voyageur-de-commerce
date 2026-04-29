#pragma once
#include "TSPInstance.h"
#include <istream>

// ---------------------------------------------------------------------------
// ExplicitMatrixReader
// ---------------------------------------------------------------------------
// Responsabilité unique : lire la section EDGE_WEIGHT_SECTION d'un fichier
// dont le format est UPPER_ROW, et remplir la matrice dist[][] de l'instance.
//
// Pourquoi une classe séparée ?
//   Le format TSPLIB supporte plusieurs formats de matrice (FULL_MATRIX,
//   LOWER_ROW, UPPER_COL, etc.). En isolant chaque format dans sa propre
//   classe, on peut ajouter un nouveau format sans toucher au parseur
//   principal (principe ouvert/fermé).
//
// Format UPPER_ROW rappel :
//   La matrice est symétrique. On ne stocke que le triangle supérieur strict
//   (sans la diagonale). La ligne k contient les distances du nœud k vers
//   tous les nœuds d'indice > k.
//
//   Pour n=4 :
//     d(0,1) d(0,2) d(0,3)
//     d(1,2) d(1,3)
//     d(2,3)
//
//   Soit (n*(n-1))/2 valeurs au total.
// ---------------------------------------------------------------------------

class ExplicitMatrixReader {
public:
    // Lit depuis le flux `in` (positionné juste après la ligne
    // "EDGE_WEIGHT_SECTION") et remplit instance.dist.
    // Lance std::runtime_error si le flux est insuffisant.
    static void read(std::istream& in, TSPInstance& instance);
};