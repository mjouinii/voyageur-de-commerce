#include "../include/TSPParser.h"
#include "../include/ExplicitMatrixReader.h"
#include "../include/CoordDistanceCalc.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

// ---------------------------------------------------------------------------
// TSPParser::load – Algorithme robuste
// ---------------------------------------------------------------------------
TSPInstance TSPParser::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("TSPParser : impossible d'ouvrir '" + filepath + "'");
    }

    TSPInstance instance;
    std::string edgeWeightType;    
    std::string edgeWeightFormat;  

    // -----------------------------------------------------------------------
    // Passe 1 : Lecture de l'en-tête
    // -----------------------------------------------------------------------
    std::string line;
    while (std::getline(file, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;

        // Détection des sections de données (on sort de la boucle d'en-tête)
        if (trimmed.find("EDGE_WEIGHT_SECTION") == 0 || 
            trimmed.find("NODE_COORD_SECTION") == 0 || 
            trimmed.find("DISPLAY_DATA_SECTION") == 0 ||
            trimmed == "EOF") {
            break; 
        }

        // Parsing des champs "CLE : VALEUR"
        size_t colon = trimmed.find(':');
        if (colon == std::string::npos) continue; 

        std::string key   = trim(trimmed.substr(0, colon));
        std::string value = trim(trimmed.substr(colon + 1));

        std::transform(key.begin(), key.end(), key.begin(), ::toupper);

        if (key == "NAME") {
            instance.name = value;
        }
        else if (key == "DIMENSION") {
            instance.n = std::stoi(value);
        }
        else if (key == "EDGE_WEIGHT_TYPE") {
            std::transform(value.begin(), value.end(), value.begin(), ::toupper);
            edgeWeightType = value;
        }
        else if (key == "EDGE_WEIGHT_FORMAT") {
            std::transform(value.begin(), value.end(), value.begin(), ::toupper);
            edgeWeightFormat = value;
        }
    }

    // Validation
    if (instance.n <= 0) {
        throw std::runtime_error("TSPParser : DIMENSION invalide dans '" + filepath + "'");
    }
    if (edgeWeightType.empty()) {
        throw std::runtime_error("TSPParser : EDGE_WEIGHT_TYPE manquant dans '" + filepath + "'");
    }

    // -----------------------------------------------------------------------
    // Passe 2 : Lecture des données
    // -----------------------------------------------------------------------
    if (edgeWeightType == "EXPLICIT") {
        if (edgeWeightFormat != "UPPER_ROW") {
            throw std::runtime_error("TSPParser : Format " + edgeWeightFormat + " non supporté");
        }
        
        // 1. On lit la matrice de distances
        ExplicitMatrixReader::read(file, instance);

        // 2. Recherche d'une éventuelle section d'affichage (pour bayg29)
        std::string nextLine;
        while (std::getline(file, nextLine)) {
            if (trim(nextLine).find("DISPLAY_DATA_SECTION") == 0) {
                CoordDistanceCalc calc(DistanceType::EUC_2D); 
                calc.read(file, instance);
                break;
            }
        }   
    }
    else if (edgeWeightType == "ATT") {
        CoordDistanceCalc calc(DistanceType::ATT);
        calc.read(file, instance);
    }
    else if (edgeWeightType == "EUC_2D") {
        CoordDistanceCalc calc(DistanceType::EUC_2D);
        calc.read(file, instance);
    }
    else {
        throw std::runtime_error("TSPParser : Type " + edgeWeightType + " non supporté");
    }

    return instance;
}

// ---------------------------------------------------------------------------
// Utilitaires
// ---------------------------------------------------------------------------

std::string TSPParser::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string TSPParser::extractValue(const std::string& line) {
    size_t colon = line.find(':');
    if (colon == std::string::npos) return "";
    return trim(line.substr(colon + 1));
}