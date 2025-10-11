#include "ppm_app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 200
#define STYLE_RESULTAT "\033[1;92m"
#define STYLE_ERROR "\033[1;4;91m"
#define STYLE_ERROR_SENT "\033[1;91m"
#define REINITIALISER "\033[0m"

int main(void) {
    char input[MAX_INPUT];

    printf("\nBienvenue dans l'application ppmviewer!\n");
    printf("Entrez 'help' pour obtenir de l'aide et 'quit' pour quitter.\n\n");

    while (1) {
        printf("ppmviewer> ");
        fflush(stdout); // s'assurer que ppmviewer> s'affiche directement.

        if (!fgets(input, sizeof(input), stdin)) {
            printf("\nProgramme interrompu.\n");
            break;
        }

        // Supprimer le retour chariot
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) continue;

        // Découper la commande
        char *commande = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, " ");
        char *arg3 = strtok(NULL, " ");
        char *arg4 = strtok(NULL, " ");
        char *arg5 = strtok(NULL, " ");
        char *arg6 = strtok(NULL, " ");

        if (!commande) continue;

        // ---- QUIT ----
        if (strcmp(commande, "quit") == 0) {
            printf("Fin du programme.\n");
            break;
        }

        // ---- SIZE ----
        else if (strcmp(commande, "size") == 0) {
            if (!arg1) {
                printf(STYLE_ERROR "Erreur : 'size' nécessite un nom de fichier.\n" REINITIALISER);
                continue;
            }

            InfoPix *infos = infoShow(arg1);
            if (!infos) {
                printf(STYLE_ERROR "Erreur : impossible de lire les informations du fichier %s.\n" REINITIALISER, arg1);
                continue;
            }

            printf(STYLE_RESULTAT "%d x %d\n" REINITIALISER, infos->largeur, infos->hauteur);
        }

        // ---- CUT ----
        else if (strcmp(commande, "cut") == 0) {
            if (!arg1 || !arg2 || !arg3 || !arg4 || !arg5 || !arg6) {
                printf(STYLE_ERROR "Erreur : 'cut' nécessite 6 arguments.\n" REINITIALISER);
                printf("Usage : cut <image.ppm> <l1> <l2> <c1> <c2> <sortie.ppm>\n");
                continue;
            }

            int l1 = atoi(arg2);
            int l2 = atoi(arg3);
            int c1 = atoi(arg4);
            int c2 = atoi(arg5);

            cut(arg1, l1, l2, c1, c2, arg6);
        }

        // ---- DOM ----
        else if (strcmp(commande, "dom") == 0) {
            if (!arg1 || !arg2 || !arg3) {
                printf(STYLE_ERROR "Erreur : 'dom' nécessite 3 arguments.\n" REINITIALISER);
                printf("Usage : dom <R|V|B> <valeur> <image.ppm>\n");
                continue;
            }

            char dominante = arg1[0];
            int valeur = atoi(arg2);
            foncerEclaircirDominante(dominante, valeur, arg3);
        }

        // ---- NEG ----
        else if (strcmp(commande, "neg") == 0) {
            if (!arg1 || !arg2) {
                printf(STYLE_ERROR "Erreur : 'neg' nécessite 2 arguments.\n" REINITIALISER);
                printf("Usage : neg <imageSource.ppm> <imageSortie.ppm>\n");
                continue;
            }

            creerNegatif(arg1, arg2);
        }

        // ---- GRIS ----
        else if (strcmp(commande, "gris") == 0) {
            if (!arg1) {
                printf(STYLE_ERROR "Erreur : 'gris' nécessite un argument.\n" REINITIALISER);
                printf("Usage : gris <imageSource.ppm>\n");
                continue;
            }

            convertirEnGris(arg1);
        }

        // ---- FILTRE MEDIAN ----
        else if (strcmp(commande, "fil") == 0) {
            if (!arg1 || !arg2) {
                printf(STYLE_ERROR "Erreur : 'fil' nécessite 2 arguments.\n" REINITIALISER);
                printf("Usage : fil <imageSource.ppm> <imageSortie.ppm>\n");
                continue;
            }

            filtre_median(arg1, arg2);
        }

        // ---- HELP ----
        else if (strcmp(commande, "help") == 0) {
            printf("\n--- Liste des commandes disponibles ---\n\n");
            printf("size <fichier.ppm>                 : Affiche la taille de l'image.\n");
            printf("cut <image> <l1> <l2> <c1> <c2> <sortie> : Coupe une partie de l'image.\n");
            printf("dom <R|V|B> <valeur> <image>      : Fonce ou éclaircit une dominante.\n");
            printf("neg <source> <sortie>             : Crée le négatif de l'image.\n");
            printf("gris <source>                     : Convertit l'image en niveaux de gris.\n");
            printf("fil <source> <sortie>             : Applique le filtre médian.\n");
            printf("quit                              : Quitte l'application.\n\n");
        }

        // ---- COMMANDE INCONNUE ----
        else {
            printf(STYLE_ERROR_SENT "Commande inconnue : " STYLE_ERROR "%s\n" REINITIALISER, commande);
            printf("Entrez 'help' pour obtenir de l'aide.\n");
        }
    }

    return 0;
}
