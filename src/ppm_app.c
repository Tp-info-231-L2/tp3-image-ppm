#include "ppm_app.h"

// Implementation de la fonction infoShow().
InfoPix *infoShow(const char *nomFichier) {
    InfoPix *infos = malloc(sizeof(*infos));
    if (!infos) {
        fprintf(stderr, "Erreur : allocation mémoire.\n");
        exit(EXIT_FAILURE);
    }

    FILE *fileOut = fopen(nomFichier, "r");
    if (!fileOut) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s.\n", nomFichier);
        free(infos);
        exit(EXIT_FAILURE);
    }

    // Lecture du format (ex : "P3")
    fscanf(fileOut, "%2s", infos->format);

    int c;
    // Ignorer espaces, tabulations, retours à la ligne
    do {
        c = fgetc(fileOut);
    } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');

    // Ignorer les commentaires
    while (c == '#') {
        char ligne[256];
        fgets(ligne, sizeof(ligne), fileOut);
        c = fgetc(fileOut);
    }
    ungetc(c, fileOut);

    // Lecture des dimensions et de la valeur max
    fscanf(fileOut, "%d %d", &infos->largeur, &infos->hauteur);
    fscanf(fileOut, "%d", &infos->valeurMax);

    fclose(fileOut);
    return infos;
}

// Implémentation de la fonction foncerEclaircirDominante().
void foncerEclaircirDominante(char couleurDominante, int valeur, const char *nomFichier) {
    InfoPix *infos = infoShow(nomFichier);

    FILE *f_in = fopen(nomFichier, "r");
    if (!f_in) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s.\n", nomFichier);
        free(infos);
        return;
    }

    // Ignorer l'en-tête (déjà lue dans infoShow)
    char ligne[256];
    fgets(ligne, sizeof(ligne), f_in); // format
    while (fgets(ligne, sizeof(ligne), f_in)) {
        if (ligne[0] != '#' && strlen(ligne) > 1) break;
    }

    // On saute jusqu'à la ligne contenant la valeur max
    while (ligne[0] == '#' || strlen(ligne) <= 1) fgets(ligne, sizeof(ligne), f_in);
    int valeurMax = infos->valeurMax;

    // Préparation du fichier de sortie
    char nomSortie[256];
    snprintf(nomSortie, sizeof(nomSortie), "%s_dom.ppm", nomFichier);
    FILE *f_out = fopen(nomSortie, "w");
    if (!f_out) {
        fprintf(stderr, "Erreur : impossible de créer %s.\n", nomSortie);
        fclose(f_in);
        free(infos);
        return;
    }

    // Réécrire l'en-tête
    fprintf(f_out, "%s\n%d %d\n%d\n", infos->format, infos->largeur, infos->hauteur, infos->valeurMax);

    // Lecture et modification des pixels
    Pixel p;
    int totalPixels = infos->largeur * infos->hauteur;

    for (int i = 0; i < totalPixels; i++) {
        if (fscanf(f_in, "%d %d %d", &p.r, &p.g, &p.b) != 3) break;

        // Vérifier la dominante
        int estDominant = 0;
        if (couleurDominante == 'R' && p.r > p.g && p.r > p.b) estDominant = 1;
        else if (couleurDominante == 'G' && p.g > p.r && p.g > p.b) estDominant = 1;
        else if (couleurDominante == 'B' && p.b > p.r && p.b > p.g) estDominant = 1;

        if (estDominant) {
            p.r += valeur;
            p.g += valeur;
            p.b += valeur;

            // Clamp pour rester entre 0 et valeurMax
            if (p.r < 0) p.r = 0; if (p.r > valeurMax) p.r = valeurMax;
            if (p.g < 0) p.g = 0; if (p.g > valeurMax) p.g = valeurMax;
            if (p.b < 0) p.b = 0; if (p.b > valeurMax) p.b = valeurMax;
        }

        fprintf(f_out, "%d %d %d ", p.r, p.g, p.b);
    }

    fclose(f_in);
    fclose(f_out);
    free(infos);

    printf("Opération effectuée ; %s créé.\n", nomSortie);
}

// Implémentation de la fonction convertirEnGris().
void convertirEnGris(const char *nomFichier) {
    InfoPix *infos = infoShow(nomFichier);

    FILE *f_in = fopen(nomFichier, "r");
    if (!f_in) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s.\n", nomFichier);
        free(infos);
        return;
    }

    // Ignorer l'en-tête déjà lue
    char ligne[256];
    fgets(ligne, sizeof(ligne), f_in); // format
    while (fgets(ligne, sizeof(ligne), f_in)) {
        if (ligne[0] != '#' && strlen(ligne) > 1) break;
    }

    // Aller jusqu'à la valeur max
    while (ligne[0] == '#' || strlen(ligne) <= 1)
        fgets(ligne, sizeof(ligne), f_in);

    int valeurMax = infos->valeurMax;

    // Créer le fichier de sortie
    char nomSortie[256];
    snprintf(nomSortie, sizeof(nomSortie), "%s_gris.ppm", nomFichier);
    FILE *f_out = fopen(nomSortie, "w");
    if (!f_out) {
        fprintf(stderr, "Erreur : impossible de créer %s.\n", nomSortie);
        fclose(f_in);
        free(infos);
        return;
    }

    // Réécrire l'en-tête
    fprintf(f_out, "%s\n%d %d\n%d\n", infos->format, infos->largeur, infos->hauteur, valeurMax);

    Pixel p;
    int totalPixels = infos->largeur * infos->hauteur;

    for (int i = 0; i < totalPixels; i++) {
        if (fscanf(f_in, "%d %d %d", &p.r, &p.g, &p.b) != 3) break;

        // Calcul du niveau de gris
        int gris = (p.r + p.g + p.b) / 3;

        // Remplacer les trois composantes par la moyenne
        p.r = p.g = p.b = gris;

        fprintf(f_out, "%d %d %d ", p.r, p.g, p.b);
    }

    fclose(f_in);
    fclose(f_out);
    free(infos);

    printf("Opération effectuée ; %s créé.\n", nomSortie);
}

// Implémentation de la fonction creerNegatif().
void creerNegatif(const char *nomFichierEntree, const char *nomFichierSortie) {
    InfoPix *infos = infoShow(nomFichierEntree);

    FILE *f_in = fopen(nomFichierEntree, "r");
    if (!f_in) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s.\n", nomFichierEntree);
        free(infos);
        return;
    }

    // Ignorer l'en-tête déjà lue
    char ligne[256];
    fgets(ligne, sizeof(ligne), f_in); // format
    while (fgets(ligne, sizeof(ligne), f_in)) {
        if (ligne[0] != '#' && strlen(ligne) > 1) break;
    }

    // Aller jusqu'à la valeur max
    while (ligne[0] == '#' || strlen(ligne) <= 1)
        fgets(ligne, sizeof(ligne), f_in);

    int valeurMax = infos->valeurMax;

    FILE *f_out = fopen(nomFichierSortie, "w");
    if (!f_out) {
        fprintf(stderr, "Erreur : impossible de créer %s.\n", nomFichierSortie);
        fclose(f_in);
        free(infos);
        return;
    }

    // Réécriture de l'en-tête
    fprintf(f_out, "%s\n%d %d\n%d\n", infos->format, infos->largeur, infos->hauteur, valeurMax);

    Pixel p;
    int totalPixels = infos->largeur * infos->hauteur;

    for (int i = 0; i < totalPixels; i++) {
        if (fscanf(f_in, "%d %d %d", &p.r, &p.g, &p.b) != 3) break;

        // Inversion (négatif)
        p.r = valeurMax - p.r;
        p.g = valeurMax - p.g;
        p.b = valeurMax - p.b;

        fprintf(f_out, "%d %d %d ", p.r, p.g, p.b);
    }

    fclose(f_in);
    fclose(f_out);
    free(infos);

    printf("Opération effectuée ; %s créé.\n", nomFichierSortie);
}

// Implémentation de la fonction cut().
void cut(const char *fichier, int l1, int l2, int c1, int c2, const char *fichier_resultat) {
    InfoPix *infos = infoShow(fichier);

    // Vérification des bornes
    if (!(l1 >= 0 && l2 > l1 && l2 <= infos->hauteur &&
          c1 >= 0 && c2 > c1 && c2 <= infos->largeur)) {
        fprintf(stderr, "Erreur : coordonnées invalides.\n");
        free(infos);
        return;
    }

    FILE *fileIn = fopen(fichier, "r");
    if (!fileIn) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s.\n", fichier);
        free(infos);
        return;
    }

    // Relecture du header
    char buffer[256];
    fgets(buffer, sizeof(buffer), fileIn); // format
    int c;
    do {
        c = fgetc(fileIn);
    } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');

    while (c == '#') {
        fgets(buffer, sizeof(buffer), fileIn);
        c = fgetc(fileIn);
    }
    ungetc(c, fileIn);

    fscanf(fileIn, "%d %d", &infos->largeur, &infos->hauteur);
    fscanf(fileIn, "%d", &infos->valeurMax);

    // Allocation mémoire pour les pixels
    Pixel **pixels = malloc(infos->hauteur * sizeof(Pixel *));
    for (int i = 0; i < infos->hauteur; i++) {
        pixels[i] = malloc(infos->largeur * sizeof(Pixel));
    }

    // Lecture de tous les pixels
    for (int i = 0; i < infos->hauteur; i++) {
        for (int j = 0; j < infos->largeur; j++) {
            fscanf(fileIn, "%d %d %d", &pixels[i][j].r, &pixels[i][j].g, &pixels[i][j].b);
        }
    }
    fclose(fileIn);

    // Création du fichier résultat
    FILE *fileOut = fopen(fichier_resultat, "w");
    if (!fileOut) {
        fprintf(stderr, "Erreur : impossible de créer %s.\n", fichier_resultat);
        for (int i = 0; i < infos->hauteur; i++) free(pixels[i]);
        free(pixels);
        free(infos);
        return;
    }

    // Écrire header
    fprintf(fileOut, "%s\n", infos->format);
    fprintf(fileOut, "%d %d\n", c2 - c1, l2 - l1);
    fprintf(fileOut, "%d\n", infos->valeurMax);

    // Écriture des pixels découpés
    for (int i = l1; i < l2; i++) {
        for (int j = c1; j < c2; j++) {
            fprintf(fileOut, "%d %d %d ", pixels[i][j].r, pixels[i][j].g, pixels[i][j].b);
        }
        fprintf(fileOut, "\n");
    }

    fclose(fileOut);

    // Libération mémoire
    for (int i = 0; i < infos->hauteur; i++) free(pixels[i]);
    free(pixels);
    free(infos);

    printf("Découpage terminé. Image enregistrée dans %s\n", fichier_resultat);
}


// Implémentation de la fonction filtre_median().

// Fonction de comparaison pour qsort
int compare_int(const void *a, const void *b) {
    return (*(int*)a) - (*(int*)b);
}

void filtre_median(const char *fichier, const char *fichier_resultat) {
    InfoPix *infos = infoShow(fichier);

    FILE *fileIn = fopen(fichier, "r");
    if (!fileIn) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s.\n", fichier);
        free(infos);
        return;
    }

    // Ignorer header déjà lu
    char buffer[256];
    fgets(buffer, sizeof(buffer), fileIn);
    int c;
    do { c = fgetc(fileIn); } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');
    while (c == '#') {
        fgets(buffer, sizeof(buffer), fileIn);
        c = fgetc(fileIn);
    }
    ungetc(c, fileIn);
    fscanf(fileIn, "%d %d", &infos->largeur, &infos->hauteur);
    fscanf(fileIn, "%d", &infos->valeurMax);

    // Lecture des pixels
    Pixel **pixels = malloc(infos->hauteur * sizeof(Pixel*));
    Pixel **pixels_res = malloc(infos->hauteur * sizeof(Pixel*)); // pixels résultat
    for (int i = 0; i < infos->hauteur; i++) {
        pixels[i] = malloc(infos->largeur * sizeof(Pixel));
        pixels_res[i] = malloc(infos->largeur * sizeof(Pixel));
    }

    for (int i = 0; i < infos->hauteur; i++) {
        for (int j = 0; j < infos->largeur; j++) {
            fscanf(fileIn, "%d %d %d", &pixels[i][j].r, &pixels[i][j].g, &pixels[i][j].b);
        }
    }
    fclose(fileIn);

    // Application du filtre médian
    int voisinR[9], voisinG[9], voisinB[9];
    for (int i = 0; i < infos->hauteur; i++) {
        for (int j = 0; j < infos->largeur; j++) {
            int k = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < infos->hauteur && nj >= 0 && nj < infos->largeur) {
                        voisinR[k] = pixels[ni][nj].r;
                        voisinG[k] = pixels[ni][nj].g;
                        voisinB[k] = pixels[ni][nj].b;
                        k++;
                    }
                }
            }
            // Trier et trouver la médiane
            qsort(voisinR, k, sizeof(int), compare_int);
            qsort(voisinG, k, sizeof(int), compare_int);
            qsort(voisinB, k, sizeof(int), compare_int);
            pixels_res[i][j].r = voisinR[k/2];
            pixels_res[i][j].g = voisinG[k/2];
            pixels_res[i][j].b = voisinB[k/2];
        }
    }

    // Écrire fichier résultat
    FILE *fileOut = fopen(fichier_resultat, "w");
    if (!fileOut) {
        fprintf(stderr, "Erreur : impossible de créer %s.\n", fichier_resultat);
        for (int i = 0; i < infos->hauteur; i++) {
            free(pixels[i]);
            free(pixels_res[i]);
        }
        free(pixels);
        free(pixels_res);
        free(infos);
        return;
    }

    fprintf(fileOut, "%s\n", infos->format);
    fprintf(fileOut, "%d %d\n", infos->largeur, infos->hauteur);
    fprintf(fileOut, "%d\n", infos->valeurMax);

    for (int i = 0; i < infos->hauteur; i++) {
        for (int j = 0; j < infos->largeur; j++) {
            fprintf(fileOut, "%d %d %d ", pixels_res[i][j].r, pixels_res[i][j].g, pixels_res[i][j].b);
        }
        fprintf(fileOut, "\n");
    }

    fclose(fileOut);

    // Libération mémoire
    for (int i = 0; i < infos->hauteur; i++) {
        free(pixels[i]);
        free(pixels_res[i]);
    }
    free(pixels);
    free(pixels_res);
    free(infos);

    printf("Filtre médian appliqué avec succès. Résultat dans %s\n", fichier_resultat);
}
