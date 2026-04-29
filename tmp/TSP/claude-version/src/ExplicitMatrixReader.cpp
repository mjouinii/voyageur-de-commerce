#include "../include/ExplicitMatrixReader.h"
#include <stdexcept>

// ---------------------------------------------------------------------------
// Algorithme de reconstruction de la matrice complète depuis UPPER_ROW
// ---------------------------------------------------------------------------
// Étape 1 – Allouer la matrice n×n initialisée à 0.
//           La diagonale restera à 0 (distance d'un nœud à lui-même).
//
// Étape 2 – Lire les (n*(n-1))/2 valeurs entières dans l'ordre.
//           Pour chaque paire (i, j) avec j > i :
//             dist[i][j] = valeur lue
//             dist[j][i] = même valeur  (symétrie)
//
// Étape 3 – Vérifier qu'on a bien lu toutes les valeurs attendues.
//
// Pourquoi ne pas lire ligne par ligne ?
//   Le fichier bayg29.tsp contient des espaces en tête de ligne et des
//   retours à la ligne arbitraires. En utilisant `in >> value` (extraction
//   par opérateur >>), on ignore automatiquement tout whitespace (espaces,
//   tabulations, sauts de ligne). C'est la méthode la plus robuste.
// ---------------------------------------------------------------------------

void ExplicitMatrixReader::read(std::istream& in, TSPInstance& instance) {
    const int n = instance.n;

    // --- Étape 1 : allocation ---
    // On initialise toute la matrice à 0.
    // resize(n, vector<int>(n, 0)) crée n lignes de n zéros.
    instance.dist.assign(n, std::vector<int>(n, 0));

    // --- Étape 2 : lecture et reconstruction symétrique ---
    // i parcourt les lignes du triangle supérieur (nœud source)
    // j parcourt les colonnes à droite de la diagonale (nœud destination)
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int value;
            if (!(in >> value)) {
                throw std::runtime_error(
                    "ExplicitMatrixReader : données insuffisantes à la position ("
                    + std::to_string(i) + "," + std::to_string(j) + ")"
                );
            }
            instance.dist[i][j] = value;
            instance.dist[j][i] = value;   // symétrie
        }
    }

    // --- Étape 3 : vérification optionnelle ---
    // On s'assure que la diagonale est bien à 0 (distance i→i = 0).
    // Ce n'est pas strictement nécessaire car assign() l'a déjà mis à 0,
    // mais c'est une bonne pratique défensive.
    for (int i = 0; i < n; ++i) {
        instance.dist[i][i] = 0;
    }
}