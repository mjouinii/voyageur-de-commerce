#ifndef TSP_PARSER_H
#define TSP_PARSER_H

#include "TSPInstance.h"

/*
 * tsp_load
 * --------
 * Charge un fichier .tsp au format TSPLIB dans 'inst'.
 * Retourne true si succes, false sinon.
 */
bool tsp_load(const char *filepath, TSPInstance *inst);

#endif /* TSP_PARSER_H */
