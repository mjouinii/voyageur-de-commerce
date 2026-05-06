#include "TSPInstance.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────
// CONSTRUCTEUR : lit le fichier .tsp et initialise tout
// ─────────────────────────────────────────────────────────────
TSPInstance::TSPInstance(const std::string& cheminFichier) {
    std::ifstream fichier(cheminFichier);

    // Vérifier que le fichier existe et est lisible
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + cheminFichier);
    }

    std::string ligne;
    std::string typeDistance = "";
    std::string formatPoids  = "";
    nbVilles = 0;

    // ── Lecture de l'en-tête ──────────────────────────────────
    // On parcourt le fichier ligne par ligne jusqu'à trouver une section de données
    while (std::getline(fichier, ligne)) {

        // Supprimer \r (fins de ligne Windows) et espaces autour
        if (!ligne.empty() && ligne.back() == '\r') ligne.pop_back();
        size_t debut = ligne.find_first_not_of(" \t\r\n");
        if (debut != std::string::npos) ligne = ligne.substr(debut);

        // Récupérer le nom de l'instance
        if (ligne.rfind("NAME", 0) == 0) {
            size_t pos = ligne.find(':');
            if (pos != std::string::npos)
                nom = ligne.substr(pos + 1);
            // Supprimer espaces autour du nom
            size_t d = nom.find_first_not_of(" \t");
            if (d != std::string::npos) nom = nom.substr(d);
        }

        // Récupérer le nombre de villes
        else if (ligne.rfind("DIMENSION", 0) == 0) {
            size_t pos = ligne.find(':');
            if (pos != std::string::npos)
                nbVilles = std::stoi(ligne.substr(pos + 1));
        }

        // Récupérer le type de distance (ATT, EXPLICIT, EUC_2D...)
        else if (ligne.rfind("EDGE_WEIGHT_TYPE", 0) == 0) {
            size_t pos = ligne.find(':');
            if (pos != std::string::npos) {
                typeDistance = ligne.substr(pos + 1);
                size_t d = typeDistance.find_first_not_of(" \t");
                if (d != std::string::npos) typeDistance = typeDistance.substr(d);
            }
        }

        // Récupérer le format de la matrice (UPPER_ROW, FULL_MATRIX...)
        else if (ligne.rfind("EDGE_WEIGHT_FORMAT", 0) == 0) {
            size_t pos = ligne.find(':');
            if (pos != std::string::npos) {
                formatPoids = ligne.substr(pos + 1);
                size_t d = formatPoids.find_first_not_of(" \t");
                if (d != std::string::npos) formatPoids = formatPoids.substr(d);
            }
        }

        // ── Début des données de distance (UPPER_ROW) ─────────
        else if (ligne == "EDGE_WEIGHT_SECTION") {
            lireFormatExplicite(fichier);
            break;
        }

        // ── Début des données de coordonnées ──────────────────
        else if (ligne == "NODE_COORD_SECTION") {
            lireFormatCoordonnees(fichier);
            // Construire la matrice depuis les coordonnées
            construireMatrice();
            break;
        }
    }

    fichier.close();
}

// ─────────────────────────────────────────────────────────────
// Lit la matrice UPPER_ROW de bayg29.tsp
// Format : matrice triangulaire supérieure sans la diagonale
//   ligne 0 : distances de ville 0 vers villes 1, 2, ..., n-1
//   ligne 1 : distances de ville 1 vers villes 2, ..., n-1
//   ...
// ─────────────────────────────────────────────────────────────
void TSPInstance::lireFormatExplicite(std::ifstream& fichier) {
    // Initialiser la matrice n×n à 0
    distances.assign(nbVilles, std::vector<double>(nbVilles, 0.0));

    std::string ligne;
    int i = 0; // ville source (ligne de la matrice)
    int j = 1; // ville destination (colonne de départ = i+1)

    while (std::getline(fichier, ligne) && i < nbVilles - 1) {
        if (ligne == "EOF" || ligne == "DISPLAY_DATA_SECTION") break;

        std::istringstream iss(ligne);
        double valeur;

        while (iss >> valeur) {
            // La valeur est la distance entre ville i et ville j
            distances[i][j] = valeur;
            distances[j][i] = valeur; // matrice symétrique : d(i,j) = d(j,i)

            j++;
            // Si on a rempli toute la ligne i, on passe à la ligne i+1
            if (j >= nbVilles) {
                i++;
                j = i + 1;
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────
// Lit les coordonnées x,y de att48.tsp
// Format :
//   id   x   y
//   1  6734  1453
//   2  2233    10
//   ...
// ─────────────────────────────────────────────────────────────
void TSPInstance::lireFormatCoordonnees(std::ifstream& fichier) {
    villes.resize(nbVilles);
    std::string ligne;

    for (int i = 0; i < nbVilles; i++) {
        std::getline(fichier, ligne);
        if (ligne == "EOF") break;

        std::istringstream iss(ligne);
        int id;
        double x, y;
        iss >> id >> x >> y;

        villes[i].id = id;
        villes[i].x  = x;
        villes[i].y  = y;
    }
}

// ─────────────────────────────────────────────────────────────
// Construit la matrice des distances depuis les coordonnées
// Utilisé pour att48.tsp (type ATT)
// ─────────────────────────────────────────────────────────────
void TSPInstance::construireMatrice() {
    distances.assign(nbVilles, std::vector<double>(nbVilles, 0.0));

    for (int i = 0; i < nbVilles; i++) {
        for (int j = 0; j < nbVilles; j++) {
            if (i != j) {
                distances[i][j] = distanceATT(villes[i], villes[j]);
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────
// Distance ATT (pseudo-euclidienne) — formule officielle TSPLIB
// Utilisée pour att48.tsp
// ─────────────────────────────────────────────────────────────
double TSPInstance::distanceATT(const Ville& a, const Ville& b) const {
    double xd  = a.x - b.x;
    double yd  = a.y - b.y;
    double rtt = std::sqrt((xd * xd + yd * yd) / 10.0);
    int    dij = (int) rtt;
    if (dij < rtt) dij++; // arrondi au plafond
    return (double) dij;
}

// ─────────────────────────────────────────────────────────────
// Distance euclidienne standard
// (non utilisée pour nos instances, mais disponible)
// ─────────────────────────────────────────────────────────────
double TSPInstance::distanceEuclidienne(const Ville& a, const Ville& b) const {
    double xd = a.x - b.x;
    double yd = a.y - b.y;
    return std::sqrt(xd * xd + yd * yd);
}

// ─────────────────────────────────────────────────────────────
// Retourne la distance entre ville i et ville j (0-indexé)
// ─────────────────────────────────────────────────────────────
double TSPInstance::getDistance(int i, int j) const {
    return distances[i][j];
}

// ─────────────────────────────────────────────────────────────
// Calcule la distance totale d'un chemin complet
// chemin = {0, 3, 1, 4, 2, 0}  (commence et finit au même endroit)
// ─────────────────────────────────────────────────────────────
double TSPInstance::distanceTotale(const std::vector<int>& chemin) const {
    double total = 0.0;
    for (int i = 0; i < (int)chemin.size() - 1; i++) {
        total += distances[chemin[i]][chemin[i + 1]];
    }
    return total;
}

// ─────────────────────────────────────────────────────────────
// Affiche un résumé de l'instance dans le terminal
// ─────────────────────────────────────────────────────────────
void TSPInstance::afficher() const {
    std::cout << "================================" << std::endl;
    std::cout << "Instance TSP : " << nom        << std::endl;
    std::cout << "Nombre de villes : " << nbVilles << std::endl;
    std::cout << "================================" << std::endl;
}