#pragma once
#include "TSPInstance.h"
#include <string>

// ---------------------------------------------------------------------------
// TSPParser  –  point d'entrée unique pour charger un fichier .tsp
// ---------------------------------------------------------------------------
// Rôle : lire l'en-tête TSPLIB, identifier le type de fichier, et déléguer
// la lecture des données à ExplicitMatrixReader ou CoordDistanceCalc.
//
// Utilisation :
//   TSPInstance inst = TSPParser::load("bayg29.tsp");
//   TSPInstance inst = TSPParser::load("att48.tsp");
//
// Exceptions :
//   std::runtime_error si le fichier est introuvable, mal formé,
//   ou si le type n'est pas supporté.
//
// Pourquoi une méthode statique plutôt qu'un objet ?
//   Le parseur n'a pas d'état propre à conserver entre deux appels.
//   Une méthode statique évite d'instancier un objet inutilement et rend
//   l'interface d'appel plus claire.
// ---------------------------------------------------------------------------

class TSPParser {
public:
    static TSPInstance load(const std::string& filepath);

private:
    // Supprime les espaces en début et fin d'une chaîne (trim)
    static std::string trim(const std::string& s);

    // Extrait la valeur après le ':' d'une ligne "CLE : VALEUR"
    static std::string extractValue(const std::string& line);
};