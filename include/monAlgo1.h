#pragma once

#include "TSPInstance2.h"

// Nearest Neighbor (Plus Proche Voisin)
// Trouve un chemin en allant toujours à la ville non visitée la plus proche
// Retourne un tableau d'indices (le chemin trouvé)
// Le tableau a une taille de nbVilles + 1 (retour au départ)
int* nearestNeighbor(TSPInstance* inst, int villeDepart);