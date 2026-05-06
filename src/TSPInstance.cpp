#include "TSPInstance.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────
// Fonctions privées (internes au fichier .cpp)
// non déclarées dans le .h car l'utilisateur n'en a pas besoin
// ─────────────────────────────────────────────────────────────

// Alloue la matrice distances[n][n]
static void allouerMatrice(TSPInstance* inst) {
    inst->distances = new double*[inst->nbVilles];
    for (int i = 0; i < inst->nbVilles; i++) {
        inst->distances[i] = new double[inst->nbVilles];
        for (int j = 0; j < inst->nbVilles; j++) {
            inst->distances[i][j] = 0.0;
        }
    }
}

// Distance ATT (formule officielle TSPLIB)
static double distanceATT(Ville& a, Ville& b) {
    double xd  = a.x - b.x;
    double yd  = a.y - b.y;
    double rtt = std::sqrt((xd * xd + yd * yd) / 10.0);
    int    dij = (int) rtt;
    if (dij < rtt) dij++;
    return (double) dij;
}

// Lit la matrice UPPER_ROW — pour bayg29.tsp
static void lireFormatExplicite(TSPInstance* inst, std::ifstream& fichier) {
    allouerMatrice(inst);

    std::string ligne;
    int i = 0;
    int j = 1;

    while (std::getline(fichier, ligne) && i < inst->nbVilles - 1) {
        if (!ligne.empty() && ligne.back() == '\r') ligne.pop_back();
        if (ligne == "EOF" || ligne == "DISPLAY_DATA_SECTION") break;

        std::istringstream iss(ligne);
        double valeur;

        while (iss >> valeur) {
            inst->distances[i][j] = valeur;
            inst->distances[j][i] = valeur; // symétrique

            j++;
            if (j >= inst->nbVilles) {
                i++;
                j = i + 1;
            }
        }
    }
}

// Lit les coordonnées x,y — pour att48.tsp
static void lireFormatCoordonnees(TSPInstance* inst, std::ifstream& fichier) {
    inst->villes = new Ville[inst->nbVilles];

    std::string ligne;
    for (int i = 0; i < inst->nbVilles; i++) {
        std::getline(fichier, ligne);
        if (!ligne.empty() && ligne.back() == '\r') ligne.pop_back();
        if (ligne == "EOF") break;

        std::istringstream iss(ligne);
        iss >> inst->villes[i].id >> inst->villes[i].x >> inst->villes[i].y;
    }
}

// Construit la matrice depuis les coordonnées
static void construireMatrice(TSPInstance* inst) {
    allouerMatrice(inst);

    for (int i = 0; i < inst->nbVilles; i++) {
        for (int j = 0; j < inst->nbVilles; j++) {
            if (i != j) {
                inst->distances[i][j] = distanceATT(inst->villes[i], inst->villes[j]);
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────
// Fonctions publiques (déclarées dans le .h)
// ─────────────────────────────────────────────────────────────

// Charge un fichier .tsp et remplit la struct
void charger(TSPInstance* inst, const std::string& cheminFichier) {
    // Initialisation
    inst->villes    = nullptr;
    inst->distances = nullptr;
    inst->nbVilles  = 0;

    std::ifstream fichier(cheminFichier);
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir : " + cheminFichier);
    }

    std::string ligne;

    while (std::getline(fichier, ligne)) {
        if (!ligne.empty() && ligne.back() == '\r') ligne.pop_back();
        size_t debut = ligne.find_first_not_of(" \t");
        if (debut != std::string::npos) ligne = ligne.substr(debut);

        // Nom de l'instance
        if (ligne.rfind("NAME", 0) == 0) {
            size_t pos = ligne.find(':');
            if (pos != std::string::npos) {
                inst->nom = ligne.substr(pos + 1);
                size_t d = inst->nom.find_first_not_of(" \t");
                if (d != std::string::npos) inst->nom = inst->nom.substr(d);
            }
        }

        // Nombre de villes
        else if (ligne.rfind("DIMENSION", 0) == 0) {
            size_t pos = ligne.find(':');
            if (pos != std::string::npos)
                inst->nbVilles = std::stoi(ligne.substr(pos + 1));
        }

        // Format bayg29 → distances données directement
        else if (ligne == "EDGE_WEIGHT_SECTION") {
            lireFormatExplicite(inst, fichier);
            break;
        }

        // Format att48 → coordonnées à calculer
        else if (ligne == "NODE_COORD_SECTION") {
            lireFormatCoordonnees(inst, fichier);
            construireMatrice(inst);
            break;
        }
    }

    fichier.close();
}

// Retourne la distance entre ville i et ville j
double getDistance(TSPInstance* inst, int i, int j) {
    return inst->distances[i][j];
}

// Calcule la distance totale d'un chemin
double distanceTotale(TSPInstance* inst, int* chemin, int taille) {
    double total = 0.0;
    for (int i = 0; i < taille - 1; i++) {
        total += inst->distances[chemin[i]][chemin[i + 1]];
    }
    return total;
}

// Affiche un résumé de l'instance
void afficher(TSPInstance* inst) {
    std::cout << "================================" << std::endl;
    std::cout << "Instance TSP : " << inst->nom          << std::endl;
    std::cout << "Nombre de villes : " << inst->nbVilles << std::endl;
    std::cout << "================================" << std::endl;
}

// Libère toute la mémoire allouée
void libererMemoire(TSPInstance* inst) {
    if (inst->distances != nullptr) {
        for (int i = 0; i < inst->nbVilles; i++) {
            delete[] inst->distances[i];
        }
        delete[] inst->distances;
        inst->distances = nullptr;
    }

    if (inst->villes != nullptr) {
        delete[] inst->villes;
        inst->villes = nullptr;
    }
}