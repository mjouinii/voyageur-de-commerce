#pragma once

#include <string>
#include <fstream>

// ─────────────────────────────────────────
// Représente une ville avec ses coordonnées
// ─────────────────────────────────────────
struct Ville {
    int    id;   // numéro de la ville (1, 2, 3...)
    double x;    // coordonnée x
    double y;    // coordonnée y
};

// ─────────────────────────────────────────
// Représente une instance du problème TSP
// ─────────────────────────────────────────
struct TSPInstance {
    std::string nom;        // nom de l'instance ("att48" ou "bayg29")
    int         nbVilles;   // nombre de villes
    Ville*      villes;     // tableau dynamique de villes
    double**    distances;  // matrice dynamique des distances [i][j]
};

// ── Fonctions qui travaillent sur TSPInstance ──────────────────

// Charge un fichier .tsp et remplit la struct
void charger(TSPInstance* inst, const std::string& cheminFichier);

// Retourne la distance entre ville i et ville j
double getDistance(TSPInstance* inst, int i, int j);

// Calcule la distance totale d'un chemin
double distanceTotale(TSPInstance* inst, int* chemin, int taille);

// Affiche un résumé dans le terminal
void afficher(TSPInstance* inst);

// Libère la mémoire (à appeler à la fin)
void libererMemoire(TSPInstance* inst);