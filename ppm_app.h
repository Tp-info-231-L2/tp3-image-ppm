#ifndef PPM_APP_H
#define PPM_APP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Represente les informations devant etre recuperees dans le fichier image.ppm
 */
typedef struct InfoPix InfoPix;
struct InfoPix {
    char format[3];    /**< format du fichier */
    int largeur;    /**< Largeur de l'image */
    int hauteur;    /**< Hauteur de l'image */
    int valeurMax;  /**< Valeur maximale de chaque coordonnées de couleurs pour un pixels */
};

/**
 * @brief Represente les couleurs d'un pixels.
 */
typedef struct Pixel Pixel;
struct Pixel {
    int r, g, b;
};

/**
 * @brief Récupère les informations nécéssaires pour la manipulations d'une image.ppm 
 * @param nomFichier Nom du fichier ou recuperer toutes ces informations.
 * @return Un pointeur de type InfoPix pointant sur toutes les informations sollicitees. 
 */
InfoPix* infoShow(const char *nomFichier);

/**
 * @brief Foncer ou éclaircir tous les pixels ayant une dominante de couleur donnée.
 * 
 * @param nomFichier Nom du fichier .ppm à traiter.
 * @param couleurDominante Caractère représentant la couleur dominante ('R', 'G', 'B').
 * @param valeur Valeur entière à ajouter (si positive) ou retrancher (si négative).
 */
void foncerEclaircirDominante(char couleurDominante, int valeur, const char *nomFichier);

/**
 * @brief Convertit une image PPM en niveaux de gris.
 * 
 * @param nomFichier Nom du fichier PPM à traiter.
 */
void convertirEnGris(const char *nomFichier);

/**
 * @brief Crée le négatif d'une image PPM (inversion des couleurs).
 * 
 * @param nomFichierEntree Nom du fichier PPM original.
 * @param nomFichierSortie Nom du fichier de sortie pour le négatif.
 */
void creerNegatif(const char *nomFichierEntree, const char *nomFichierSortie);

/**
 * @brief Découpe une partie d'une image PPM.
 * @param fichier Source du fichier image (.ppm).
 * @param l1 Ligne de début (incluse).
 * @param l2 Ligne de fin (exclue).
 * @param c1 Colonne de début (incluse).
 * @param c2 Colonne de fin (exclue).
 * @param fichier_resultat Nom du fichier de sortie.
 */
void cut(const char *fichier, int l1, int l2, int c1, int c2, const char *fichier_resultat);

/**
 * @brief Applique un filtre médian sur une image PPM.
 * @param fichier Nom du fichier image source (.ppm).
 * @param fichier_resultat Nom du fichier où enregistrer l'image filtrée.
 */
void filtre_median(const char *fichier, const char *fichier_resultat);
#endif