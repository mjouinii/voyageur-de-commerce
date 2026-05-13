#include "ExplicitMatrixReader.h"
#include "TSPInstance.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>

/* ------------------------------------------------------------------ */
/* MDS classique : matrice de distances -> coordonnees 2D             */
/*                                                                    */
/* 1. D2 = matrice des distances au carre                             */
/* 2. B  = -0.5 * H * D2 * H  (double centrage)                       */
/* 3. Power iteration pour extraire les 2 plus grandes valeurs        */
/*    propres et leurs vecteurs propres associes                      */
/* 4. coords[i] = (sqrt(l1)*v1[i], sqrt(l2)*v2[i])                    */
/* ------------------------------------------------------------------ */
static void mds_2d(TSPInstance *inst) {
    int n = inst->n;

    /* 1. Matrice D2 : distances au carre */
    double **D2 = (double **)std::malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        D2[i] = (double *)std::malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            double d = (double)inst->dist[i][j];
            D2[i][j] = d * d;
        }
    }

    /* 2. Double centrage : B_ij = -0.5*(D2_ij - rowMean_i - colMean_j + grandMean) */
    double *rowMean = (double *)std::calloc(n, sizeof(double));
    double *colMean = (double *)std::calloc(n, sizeof(double));
    double grandMean = 0.0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            rowMean[i] += D2[i][j];
    for (int i = 0; i < n; i++) rowMean[i] /= n;

    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
            colMean[j] += D2[i][j];
    for (int j = 0; j < n; j++) colMean[j] /= n;

    for (int i = 0; i < n; i++) grandMean += rowMean[i];
    grandMean /= n;

    double **B = (double **)std::malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        B[i] = (double *)std::malloc(n * sizeof(double));
        for (int j = 0; j < n; j++)
            B[i][j] = -0.5 * (D2[i][j] - rowMean[i] - colMean[j] + grandMean);
    }

    /* Libere D2, rowMean, colMean */
    for (int i = 0; i < n; i++) std::free(D2[i]);
    std::free(D2);
    std::free(rowMean);
    std::free(colMean);

    /* 3a. Vecteur propre 1 : power iteration standard */
    double *v1  = (double *)std::malloc(n * sizeof(double));
    double *tmp = (double *)std::malloc(n * sizeof(double));

    for (int i = 0; i < n; i++) v1[i] = std::cos((double)(i + 1));

    /* Normalisation initiale */
    {
        double norm = 0.0;
        for (int i = 0; i < n; i++) norm += v1[i] * v1[i];
        norm = std::sqrt(norm);
        if (norm > 1e-12) for (int i = 0; i < n; i++) v1[i] /= norm;
    }

    double lambda1 = 0.0;
    for (int iter = 0; iter < 2000; iter++) {
        for (int i = 0; i < n; i++) {
            tmp[i] = 0.0;
            for (int j = 0; j < n; j++) tmp[i] += B[i][j] * v1[j];
        }
        double norm = 0.0;
        for (int i = 0; i < n; i++) norm += tmp[i] * tmp[i];
        norm = std::sqrt(norm);
        if (norm < 1e-12) break;
        lambda1 = norm;
        for (int i = 0; i < n; i++) v1[i] = tmp[i] / norm;
    }

    /* 3b. Vecteur propre 2 : power iteration sur B avec reorthogonalisation */
    /*     contre v1 a chaque etape (Gram-Schmidt). Plus stable que la       */
    /*     deflation explicite.                                              */
    double *v2 = (double *)std::malloc(n * sizeof(double));

    /* Initialisation reproductible */
    for (int i = 0; i < n; i++) v2[i] = std::sin((double)(i + 1));

    /* Orthogonalisation initiale contre v1 */
    {
        double dot = 0.0;
        for (int i = 0; i < n; i++) dot += v2[i] * v1[i];
        for (int i = 0; i < n; i++) v2[i] -= dot * v1[i];
        double norm = 0.0;
        for (int i = 0; i < n; i++) norm += v2[i] * v2[i];
        norm = std::sqrt(norm);
        if (norm > 1e-12) for (int i = 0; i < n; i++) v2[i] /= norm;
    }

    double lambda2 = 0.0;
    for (int iter = 0; iter < 2000; iter++) {
        /* tmp = B * v2 */
        for (int i = 0; i < n; i++) {
            tmp[i] = 0.0;
            for (int j = 0; j < n; j++) tmp[i] += B[i][j] * v2[j];
        }

        /* Reorthogonalisation contre v1 : tmp -= (tmp . v1) * v1 */
        double dot = 0.0;
        for (int i = 0; i < n; i++) dot += tmp[i] * v1[i];
        for (int i = 0; i < n; i++) tmp[i] -= dot * v1[i];

        /* Normalisation */
        double norm = 0.0;
        for (int i = 0; i < n; i++) norm += tmp[i] * tmp[i];
        norm = std::sqrt(norm);
        if (norm < 1e-12) break;
        lambda2 = norm;
        for (int i = 0; i < n; i++) v2[i] = tmp[i] / norm;
    }

    /* 4. Coordonnees finales */
    double s1 = (lambda1 > 0) ? std::sqrt(lambda1) : 1.0;
    double s2 = (lambda2 > 0) ? std::sqrt(lambda2) : 1.0;

    for (int i = 0; i < n; i++) {
        inst->coords[i].x = s1 * v1[i];
        inst->coords[i].y = s2 * v2[i];
    }

    /* Nettoyage */
    for (int i = 0; i < n; i++) std::free(B[i]);
    std::free(B);
    std::free(v1);
    std::free(v2);
    std::free(tmp);
}

/* ------------------------------------------------------------------ */
/* Lecture du format UPPER_ROW puis calcul des coordonnees 2D via MDS */
/* ------------------------------------------------------------------ */
bool read_explicit_upper_row(FILE *file, TSPInstance *inst) {
    int n = inst->n;

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            int v;
            if (std::fscanf(file, "%d", &v) != 1) {
                std::fprintf(stderr,
                    "[ExplicitMatrixReader] Erreur : valeur manquante "
                    "a la position (%d, %d).\n", i, j);
                return false;
            }
            inst->dist[i][j] = v;
            inst->dist[j][i] = v;
        }
    }

    /* Calcul des coordonnees 2D par MDS pour la visualisation */
    if (inst->coords == nullptr) {
        if (!tsp_alloc_coords(inst)) {
            std::fprintf(stderr,
                "[ExplicitMatrixReader] Erreur allocation coords.\n");
            return false;
        }
    }
    mds_2d(inst);

    return true;
}
