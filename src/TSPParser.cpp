#include "TSPParser.h"
#include "ExplicitMatrixReader.h"
#include "CoordDistanceCalc.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

static const int MAX_LINE = 512;

// ── Utilitaires sur les chaînes ──────────────────────────────────────────────

// Avance le pointeur jusqu'au premier caractère non-blanc
static const char *str_trim_left(const char *s) {
    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n')
        s++;
    return s;
}

// Supprime les espaces/retours en fin de chaîne (modification en place)
static void str_trim_right_inplace(char *s) {
    int len = static_cast<int>(std::strlen(s));
    for (int i = len - 1; i >= 0; i--) {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')
            s[i] = '\0';
        else
            break;
    }
}

// Convertit une chaîne en majuscules (modification en place)
static void str_to_upper_inplace(char *s) {
    while (*s != '\0') {
        if (*s >= 'a' && *s <= 'z')
            *s = static_cast<char>(*s - 'a' + 'A');
        s++;
    }
}

// Teste si 'str' commence par 'prefix'
static bool str_starts_with(const char *str, const char *prefix) {
    while (*prefix != '\0') {
        if (*str != *prefix) return false;
        str++;
        prefix++;
    }
    return true;
}

// Retourne un pointeur vers la valeur après ':' dans "CLE : VALEUR"
// Retourne nullptr si ':' est absent
static const char *parse_value(const char *line) {
    const char *colon = std::strchr(line, ':');
    if (colon == nullptr) return nullptr;
    return str_trim_left(colon + 1);
}

// ── Parsing principal ────────────────────────────────────────────────────────

bool tsp_load(const char *filepath, TSPInstance *inst) {

    // Initialisation complète de l'instance
    std::memset(inst->name, 0, sizeof(inst->name));
    inst->n             = 0;
    inst->weight_type   = WEIGHT_TYPE_UNKNOWN;
    inst->weight_format = WEIGHT_FORMAT_UNKNOWN;
    inst->dist          = nullptr;
    inst->coords        = nullptr;
    inst->has_coords    = false;

    FILE *file = std::fopen(filepath, "r");
    if (file == nullptr) {
        std::fprintf(stderr, "[TSPParser] Impossible d'ouvrir '%s'.\n", filepath);
        return false;
    }

    bool found_name         = false;
    bool found_dimension    = false;
    bool found_weight_type  = false;
    bool data_section_found = false;
    bool ok                 = true;

    char line[MAX_LINE];

    while (std::fgets(line, MAX_LINE, file) != nullptr) {

        str_trim_right_inplace(line);

        // Copie en majuscules pour la comparaison des mots-clés
        char upper[MAX_LINE];
        std::strncpy(upper, line, MAX_LINE - 1);
        upper[MAX_LINE - 1] = '\0';
        str_to_upper_inplace(upper);

        const char *up = str_trim_left(upper);

        // ── Sections de données ──────────────────────────────────────────────

        if (str_starts_with(up, "EDGE_WEIGHT_SECTION")) {
            data_section_found = true;
            ok = read_explicit_upper_row(file, inst);
            break;
        }

        if (str_starts_with(up, "NODE_COORD_SECTION")) {
            data_section_found = true;
            ok = read_node_coord_section(file, inst);
            break;
        }

        if (str_starts_with(up, "EOF")) {
            break;
        }

        // ── Champs d'en-tête ─────────────────────────────────────────────────

        const char *val = parse_value(up);
        if (val == nullptr) continue;

        // Valeur originale (non convertie) pour NAME
        const char *orig_val = parse_value(str_trim_left(line));

        if (str_starts_with(up, "NAME")) {
            if (orig_val != nullptr) {
                std::strncpy(inst->name, orig_val, sizeof(inst->name) - 1);
                str_trim_right_inplace(inst->name);
            }
            found_name = true;
        }
        else if (str_starts_with(up, "DIMENSION")) {
            inst->n = std::atoi(val);
            found_dimension = true;

            // Dès que la dimension est connue, alloue la matrice de distances
            if (!tsp_alloc_matrix(inst)) {
                std::fclose(file);
                return false;
            }
        }
        else if (str_starts_with(up, "EDGE_WEIGHT_TYPE")) {
            if (str_starts_with(val, "EXPLICIT"))
                inst->weight_type = WEIGHT_TYPE_EXPLICIT;
            else if (str_starts_with(val, "EUC_2D"))
                inst->weight_type = WEIGHT_TYPE_EUC_2D;
            else if (str_starts_with(val, "ATT"))
                inst->weight_type = WEIGHT_TYPE_ATT;
            else {
                std::fprintf(stderr,
                    "[TSPParser] Type de pondération non supporté : '%s'.\n", val);
                std::fclose(file);
                return false;
            }
            found_weight_type = true;
        }
        else if (str_starts_with(up, "EDGE_WEIGHT_FORMAT")) {
            if (str_starts_with(val, "UPPER_ROW"))
                inst->weight_format = WEIGHT_FORMAT_UPPER_ROW;
        }
        else if (str_starts_with(up, "DISPLAY_DATA_SECTION")) {
            // Indique qu'une section de coordonnées d'affichage est présente
            // (traitée plus bas si rencontrée comme section de données)
            inst->has_coords = false;
        }
    }

    std::fclose(file);

    // ── Vérifications finales ────────────────────────────────────────────────

    if (!found_name) {
        std::fprintf(stderr,
            "[TSPParser] Attention : champ NAME absent dans '%s'.\n", filepath);
    }

    if (!found_dimension) {
        std::fprintf(stderr,
            "[TSPParser] Erreur : champ DIMENSION absent dans '%s'.\n", filepath);
        tsp_free(inst);
        return false;
    }

    if (!found_weight_type) {
        std::fprintf(stderr,
            "[TSPParser] Erreur : champ EDGE_WEIGHT_TYPE absent dans '%s'.\n", filepath);
        tsp_free(inst);
        return false;
    }

    if (!data_section_found) {
        std::fprintf(stderr,
            "[TSPParser] Erreur : aucune section de données trouvée dans '%s'.\n", filepath);
        tsp_free(inst);
        return false;
    }

    if (!ok) {
        tsp_free(inst);
        return false;
    }

    return tsp_is_valid(inst);
}
