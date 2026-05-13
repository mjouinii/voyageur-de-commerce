#ifndef TSP_INSTANCE_H
#define TSP_INSTANCE_H

const int WEIGHT_TYPE_EXPLICIT = 0;
const int WEIGHT_TYPE_EUC_2D   = 1;
const int WEIGHT_TYPE_ATT      = 2;
const int WEIGHT_TYPE_UNKNOWN  = -1;

const int WEIGHT_FORMAT_UPPER_ROW = 0;
const int WEIGHT_FORMAT_UNKNOWN   = -1;

struct NodeCoord {
    double x;
    double y;
};

struct TSPInstance {
    char   name[256];
    int    n;
    int    weight_type;
    int    weight_format;
    int  **dist;
    NodeCoord *coords;
};

bool tsp_alloc_matrix(TSPInstance *inst);
bool tsp_alloc_coords(TSPInstance *inst);
void tsp_free(TSPInstance *inst);
bool tsp_is_valid(const TSPInstance *inst);

#endif /* TSP_INSTANCE_H */
