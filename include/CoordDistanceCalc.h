#ifndef COORD_DISTANCE_CALC_H
#define COORD_DISTANCE_CALC_H

#include "TSPInstance.h"
#include <cstdio>

/*
 * Lit NODE_COORD_SECTION depuis 'file', stocke les coordonnées dans
 * inst->coords et calcule la matrice inst->dist[][] selon weight_type.
 * Métriques supportées : EUC_2D et ATT.
 */
bool read_node_coord_section(FILE *file, TSPInstance *inst);

// Calcule la distance euclidienne arrondie entre les villes i et j
int compute_distance_euc2d(const TSPInstance *inst, int i, int j);

// Calcule la distance ATT entre les villes i et j
int compute_distance_att(const TSPInstance *inst, int i, int j);

#endif /* COORD_DISTANCE_CALC_H */
