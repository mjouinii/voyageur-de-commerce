#ifndef EXPLICIT_MATRIX_READER_H
#define EXPLICIT_MATRIX_READER_H

#include "TSPInstance.h"
#include <cstdio>

/*
 * Lit les (n*(n-1))/2 entiers du format UPPER_ROW depuis 'file' et
 * reconstruit la matrice symetrique complete dans inst->dist[][].
 */
bool read_explicit_upper_row(FILE *file, TSPInstance *inst);

#endif /* EXPLICIT_MATRIX_READER_H */
