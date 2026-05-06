#pragma once

#include <vector>
#include <string>

struct Ville {
    int    id;
    double x;
    double y;
};

class TSPInstance {
public:
    std::string nom;
    int         nbVilles;
    std::vector<Ville> villes;
    std::vector<std::vector<double>> distances;

    TSPInstance(const std::string& cheminFichier);
    double getDistance(int i, int j) const;
    double distanceTotale(const std::vector<int>& chemin) const;
    void afficher() const;

private:
    void lireFormatExplicite(std::ifstream& fichier);
    void lireFormatCoordonnees(std::ifstream& fichier);
    double distanceATT(const Ville& a, const Ville& b) const;
    double distanceEuclidienne(const Ville& a, const Ville& b) const;
    void construireMatrice();
};