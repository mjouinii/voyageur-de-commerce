#include "monAlgo2.h"

// ─────────────────────────────────────────────────────────────
// Inverse le segment du chemin entre les indices gauche et droit
// Ex: chemin = {0,1,2,3,4,0}, gauche=1, droit=3
//   → chemin = {0,3,2,1,4,0}
// ─────────────────────────────────────────────────────────────
static void inverserSegment(int* chemin, int gauche, int droit) {
    while (gauche < droit) {
        int tmp         = chemin[gauche];
        chemin[gauche]  = chemin[droit];
        chemin[droit]   = tmp;
        gauche++;
        droit--;
    }
}

// ─────────────────────────────────────────────────────────────
// 2-opt
//
// Principe :
//   On essaie toutes les paires d'aretes (i, j) du chemin.
//   Pour chaque paire, on calcule si inverser le segment
//   entre i+1 et j raccourcit le chemin.
//   Si oui → on garde l'inversion et on recommence depuis le debut.
//   On s'arrete quand aucune inversion n'ameliore plus le chemin.
//
// Parametres :
//   inst   → l'instance TSP (distances)
//   chemin → le chemin a ameliorer (modifie directement)
//   taille → nbVilles + 1 (retour au depart inclus)
// ─────────────────────────────────────────────────────────────
void twoOpt(TSPInstance* inst, int* chemin, int taille) {

    int n = taille - 1; // nombre de villes (sans le retour)
    bool amelioration = true;

    while (amelioration) {
        amelioration = false;

        for (int i = 0; i < n - 1; i++) {
            for (int j = i + 2; j < n; j++) {

                // Aretes actuelles :
                //   chemin[i]   → chemin[i+1]
                //   chemin[j]   → chemin[j+1]
                int a = chemin[i];
                int b = chemin[i + 1];
                int c = chemin[j];
                int d = chemin[j + 1];

                int distActuelle  = inst->dist[a][b] + inst->dist[c][d];

                // Aretes apres inversion :
                //   chemin[i]   → chemin[j]
                //   chemin[i+1] → chemin[j+1]
                int distNouvelle  = inst->dist[a][c] + inst->dist[b][d];

                // Si c'est plus court → on inverse le segment
                if (distNouvelle < distActuelle) {
                    inverserSegment(chemin, i + 1, j);
                    amelioration = true;
                }
            }
        }
    }
}