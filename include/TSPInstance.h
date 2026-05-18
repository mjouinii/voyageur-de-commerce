#ifndef TSP_INSTANCE_H
#define TSP_INSTANCE_H

// Types de pondération supportés
const int WEIGHT_TYPE_EXPLICIT = 0;
const int WEIGHT_TYPE_EUC_2D   = 1;
const int WEIGHT_TYPE_ATT      = 2;
const int WEIGHT_TYPE_UNKNOWN  = -1;

// Formats de matrice supportés
const int WEIGHT_FORMAT_UPPER_ROW = 0;
const int WEIGHT_FORMAT_UNKNOWN   = -1;

// Coordonnées 2D d'un nœud
struct NodeCoord {
    double x;
    double y;
};

// Structure principale représentant une instance TSP
struct TSPInstance {
    char       name[256];    // Nom de l'instance
    int        n;            // Nombre de villes
    int        weight_type;  // Type de pondération (EUC_2D, ATT, EXPLICIT)
    int        weight_format;// Format de la matrice (UPPER_ROW, ...)
    int      **dist;         // Matrice de distances [n][n]
    NodeCoord *coords;       // Coordonnées des nœuds (peut être nullptr)
    bool       has_coords;   // Indique si les coordonnées sont disponibles
};

// Alloue la matrice de distances
bool tsp_alloc_matrix(TSPInstance *inst);

// Alloue le tableau de coordonnées
bool tsp_alloc_coords(TSPInstance *inst);

// Libère toute la mémoire allouée
void tsp_free(TSPInstance *inst);

// Vérifie que l'instance est valide
bool tsp_is_valid(const TSPInstance *inst);

#endif /* TSP_INSTANCE_H */
