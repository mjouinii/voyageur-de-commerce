#include "AlgoNearestNeighbor.h"
#include "TSPInstance.h"
#include <iostream>

// ─────────────────────────────────────────────────────────────
// Nearest Neighbor — Plus Proche Voisin
//
// Principe :
//   1. On part d'une ville de départ
//   2. On va toujours à la ville NON visitée la plus proche
//   3. On répète jusqu'à avoir visité toutes les villes
//   4. On revient à la ville de départ
//
// Paramètres :
//   inst        → l'instance TSP (villes + distances)
//   villeDepart → l'indice de la ville de départ (0-indexé)
//
// Retourne :
//   un tableau d'entiers représentant le chemin
//   ex: {0, 3, 1, 4, 2, 0} pour 5 villes
//   taille du tableau = nbVilles + 1
// ─────────────────────────────────────────────────────────────
int* nearestNeighbor(TSPInstance* inst, int villeDepart) {

    int n = inst->n;

    // ── Étape 1 : préparer les outils ────────────────────────

    // Tableau qui mémorise si une ville a déjà été visitée
    // visite[i] = false → ville i pas encore visitée
    // visite[i] = true  → ville i déjà visitée
    bool* visite = new bool[n];
    for (int i = 0; i < n; i++) {
        visite[i] = false;
    }

    // Tableau qui va stocker le chemin final
    // taille n+1 car on revient à la ville de départ à la fin
    int* chemin = new int[n + 1];

    // ── Étape 2 : partir de la ville de départ ───────────────

    int actuel = villeDepart;
    visite[actuel] = true;   // on marque la ville de départ comme visitée
    chemin[0] = actuel;      // on l'ajoute en premier dans le chemin

    // ── Étape 3 : boucle principale ──────────────────────────

    for (int etape = 1; etape < n; etape++) {

        int meilleureDist = -1;   // distance minimale trouvée
        int    meilleureVille = -1;  // indice de la meilleure ville

        // Chercher la ville non visitée la plus proche
        for (int j = 0; j < n; j++) {

            // On ignore les villes déjà visitées
            if (visite[j]) continue;

            int dist = inst->dist[actuel][j];

            // Si c'est la première ville non visitée
            // OU si elle est plus proche que la meilleure trouvée
            if (meilleureDist == -1 || dist < meilleureDist) {
                meilleureDist  = dist;
                meilleureVille = j;
            }
        }

        // Se déplacer vers la ville la plus proche trouvée
        actuel = meilleureVille;
        visite[actuel] = true;    // on la marque comme visitée
        chemin[etape] = actuel;   // on l'ajoute au chemin
    }

    // ── Étape 4 : retour à la ville de départ ────────────────
    chemin[n] = villeDepart;

    // ── Nettoyage ─────────────────────────────────────────────
    delete[] visite;

    return chemin;  // attention : à libérer avec delete[] dans le main
}