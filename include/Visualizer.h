#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "TSPInstance.h"

/*
 * Ouvre une fenêtre SFML et affiche les villes de l'instance.
 * Si inst->has_coords est false, la fonction ne fait rien.
 */
void visualize(const TSPInstance *inst);

#endif /* VISUALIZER_H */
