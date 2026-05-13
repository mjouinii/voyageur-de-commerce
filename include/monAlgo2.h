#pragma once

#include "TSPInstance.h"

// 2-opt : améliore un chemin existant en inversant des segments
// Modifie le tableau chemin directement
// chemin : tableau de taille n+1 (retour au départ à la fin)
void twoOpt(TSPInstance* inst, int* chemin, int taille);