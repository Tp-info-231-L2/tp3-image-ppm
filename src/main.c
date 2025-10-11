#include "ppm_app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 200
#define STYLE_RESULTAT "\033[1;92m"
#define STYLE_ERROR "\033[1;4;91m"
#define STYLE_ERROR_SENT "\033[1;91m"
#define REINITIALISER "\033[0m"

int main() {
    char input[MAX_INPUT];

    printf("\nBienvenue dans l'application ppmviewer!\n");
    printf("Entrez 'help' pour obtenir de l'aide et 'quit' pour quitter.\n\n");

    while (1) {
        printf("ppmviewer> ");
        fflush(stdout); // s'assurer que l'invite s'affiche immédiatement

        if (!fgets(input, sizeof(input), stdin)) {
            printf("\nProgramme interrompu.\n");
            break;
        }

        // Supprimer le retour chariot
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) continue; // ignorer lignes vides

        // Séparer la ligne en tokens
        char *commande = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, " ");
        char *arg3 = strtok(NULL, " ");
        char *arg4 = strtok(NULL, " ");
        char *arg5 = strtok(NULL, " ");
        char *arg6 = strtok(NULL, " ");

        if (!commande) continue;

        if (strcmp(commande, "quit") == 0) {
            printf("Fin du programme.\n");
            break;
        }
        else if (strcmp(commande, "size") == 0) {
            if (!arg1) {
                printf(STYLE_ERROR "Erreur : 'size' nécessite un nom de fichier.\n" REINITIALISER);
                continue;
            }

            int largeur = 0;
            int hauteur = 0;
            InfoPix *infos = infoShow(arg1);
            
            largeur = infos->largeur;
            hauteur = infos->hauteur;
            printf(STYLE_RESULTAT "%d x %d\n" REINITIALISER, largeur, hauteur);
        }
        else if (strcmp(commande, "cut") == 0) {
            if (!arg2 || !arg3 || !arg4 || !arg5 || !arg6) {
                printf(STYLE_ERROR "Erreur : 'cut' nécessite 6 arguments en entrée.\n" REINITIALISER);
                continue;
            }
            int ligne1 = atoi(arg2);
            int ligne2 = atoi(arg3);
            int colonne1 = atoi(arg4);
            int colonne2 = atoi(arg5);

            cut(arg1, ligne1, ligne2, colonne1, colonne2, arg6);
        }
        else if (strcmp(commande, "dom") == 0) {
            if(!arg1 || !arg2 || !arg3) {
                printf(STYLE_ERROR "La commande 'dom' nécessite 3 arguments en entrée.\n" REINITIALISER);
                continue;
            }

            char dominante = (char)arg1;
            int valeur = atoi(arg2);
            foncerEclaircirDominante(dominante, valeur, arg3);
        }
        else if (strcmp(commande, "neg") == 0) {
            if(!arg1 || !arg2) {
                printf("La commande 'neg' nécessite deux arguments en entrée (le nom du fichier source et celui du fichier de sortie).\n");
                continue;
            }

            creerNegatif(arg1, arg2);
        }
        else if (strcmp(commande, "gris") == 0) {
            if(!arg1) {
                printf("La commande 'gris' nécessite le nom du fichier source en entrée.\n");
                continue;
            }

            convertirEnGris(arg1);
        }
        else if (strcmp(commande, "fil") == 0) {
            if(!arg1 || !arg2) {
                printf("La commande 'fil' nécessite deux arguments en entrée (le nom du fichier source et celui du fichier de sortie).\n");
            }
        }
        else if (strcmp(commande, "help") == 0) {
            printf("\nListe des commandes disponibles.\n\n");
            printf("size <nomFichier> : Affiche la taille du l'image contenue dans le fichier.\n");
            printf(
                "cut <nomImage.ppm> <l1> <l2> <c1> <c2> <nomImageSortie.ppm> : Coupe les lignes entre l1 et l2 et colonnes entre c1 et c2.\n"
            );
            printf("dom <caractere> <valeur> <imageSource.ppm> : Fonce ou éclaircir la dominate d'un pixel de ImageSource.\n");
            printf("neg <imageSource.ppm> <imageSortie.ppm> : crée le négatif du premier argument dans le deuxieme.\n");
            printf("gris <imageSource.ppm> : convertie l'image source en gris.\n");
            printf("fil <imageSource.ppm> <imageSortie.ppm> : applique le filtre median à image source et le résultat dans imageSortie.ppm.\n");
        }
        else {
            printf(STYLE_ERROR_SENT "Commande inconnue : " STYLE_ERROR "%s\n" REINITIALISER, commande);
            printf("Entrez 'help' pour obtenir de l'aide.\n");
        }
    }

    return 0;
}
