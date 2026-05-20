#ifndef EXPLICIT_MATRIX_READER_H
#define EXPLICIT_MATRIX_READER_H

#include "TSPInstance.h"
#include <cstdio>

/*
 * Lit les (n*(n-1))/2 entiers du format UPPER_ROW depuis 'file' et
 * reconstruit la matrice symétrique complète dans inst->dist[][].
 */
bool read_explicit_upper_row(FILE *file, TSPInstance *inst);

/*
 * Lit DISPLAY_DATA_SECTION depuis 'file' et stocke les coordonnées
 * dans inst->coords pour la visualisation.
 */
bool read_display_data_section(FILE *file, TSPInstance *inst);

#endif /* EXPLICIT_MATRIX_READER_H */