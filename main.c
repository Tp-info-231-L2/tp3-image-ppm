#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define degra "picture.ppm"
typedef struct {
    int r, g, b;
} Pixel;

typedef struct {
    char type[3];
    int largeur, hauteur, maxval;
    Pixel *pixels;
} Image;

Image *chargerImage(const char *nom) {
    FILE *f = fopen(nom, "r");
    if (!f) {
        printf(" Impossible d’ouvrir %s\n", nom);
        return NULL;
    }

    Image *img = malloc(sizeof(Image));
    fscanf(f, "%2s", img->type);
    fscanf(f, "%d %d %d", &img->largeur, &img->hauteur, &img->maxval);

    img->pixels = malloc(img->largeur * img->hauteur * sizeof(Pixel));
    for (int i = 0; i < img->largeur * img->hauteur; i++)
        fscanf(f, "%d %d %d", &img->pixels[i].r, &img->pixels[i].g, &img->pixels[i].b);

    fclose(f);
    return img;
}

void sauvegarderImage(Image *img, const char *nom) {
    FILE *f = fopen(nom, "w");
    if (!f) {
        printf(" Impossible de créer %s\n", nom);
        return;
    }

    fprintf(f, "P3\n%d %d\n%d\n", img->largeur, img->hauteur, img->maxval);
    for (int i = 0; i < img->largeur * img->hauteur; i++)
        fprintf(f, "%d %d %d\n", img->pixels[i].r, img->pixels[i].g, img->pixels[i].b);

    fclose(f);
}
// fonction eclaicir/foncer
void dom(const char *couleur, int val, const char *nom) {
    Image *img = chargerImage(nom);
    if (!img) return;

    for (int i = 0; i < img->largeur * img->hauteur; i++) {
        Pixel *p = &img->pixels[i];
        int max = p->r;
        if (p->g > max) max = p->g;
        if (p->b > max) max = p->b;

        // Vérifier la dominante demandée
        int estDominant = 0;
        if ((couleur[0] == 'R' || couleur[0] == 'r') && p->r == max) estDominant = 1;
        if ((couleur[0] == 'G' || couleur[0] == 'g') && p->g == max) estDominant = 1;
        if ((couleur[0] == 'B' || couleur[0] == 'b') && p->b == max) estDominant = 1;

        if (estDominant) {
            p->r += val;
            p->g += val;
            p->b += val;
            if (p->r > img->maxval) p->r = img->maxval;
            if (p->g > img->maxval) p->g = img->maxval;
            if (p->b > img->maxval) p->b = img->maxval;
            if (p->r < 0) p->r = 0;
            if (p->g < 0) p->g = 0;
            if (p->b < 0) p->b = 0;
        }
    }

    sauvegarderImage(img, nom);
    printf(" Image %s modifiée (dominante %s, val=%d)\n", nom, couleur, val);
    free(img->pixels);
    free(img);
}

     //2. NOIR ET BLANC

void gris(const char *nom) {
    Image *img = chargerImage(nom);
    if (!img) return;

    for (int i = 0; i < img->largeur * img->hauteur; i++) {
        int g = (img->pixels[i].r + img->pixels[i].g + img->pixels[i].b) / 3;
        img->pixels[i].r = img->pixels[i].g = img->pixels[i].b = g;
    }

    sauvegarderImage(img, nom);
    printf(" Image convertie en noir et blanc : %s\n", nom);
    free(img->pixels);
    free(img);
}

    // 3. AFFICHER TAILLE
void size(const char *nom) {
    Image *img = chargerImage(nom);
    if (!img) return;
    printf(" Taille de %s : %d x %d\n", nom, img->largeur, img->hauteur);
    free(img->pixels);
    free(img);
}

   //  4. DÉCOUPER

void cut(const char *nom, int l1, int l2, int c1, int c2, const char *sortie) {
    Image *img = chargerImage(nom);
    if (!img) return;

    if (l1 < 0 || l2 > img->hauteur || c1 < 0 || c2 > img->largeur || l1 >= l2 || c1 >= c2) {
        printf(" Coordonnées invalides.\n");
        free(img->pixels);
        free(img);
        return;
    }

    Image newImg;
    newImg.largeur = c2 - c1;
    newImg.hauteur = l2 - l1;
    newImg.maxval = img->maxval;
    strcpy(newImg.type, img->type);
    newImg.pixels = malloc(newImg.largeur * newImg.hauteur * sizeof(Pixel));

    for (int i = 0; i < newImg.hauteur; i++)
        for (int j = 0; j < newImg.largeur; j++)
            newImg.pixels[i * newImg.largeur + j] =
                img->pixels[(l1 + i) * img->largeur + (c1 + j)];

    sauvegarderImage(&newImg, sortie);
    printf("  Image découpée sauvegardée : %s\n", sortie);
    free(newImg.pixels);
    free(img->pixels);
    free(img);
}


//     5. NÉGATIF

void neg(const char *nom, const char *sortie) {
    Image *img = chargerImage(nom);
    if (!img) return;

    for (int i = 0; i < img->largeur * img->hauteur; i++) {
        img->pixels[i].r = img->maxval - img->pixels[i].r;
        img->pixels[i].g = img->maxval - img->pixels[i].g;
        img->pixels[i].b = img->maxval - img->pixels[i].b;
    }

    sauvegarderImage(img, sortie);
    printf(" Négatif créé : %s\n", sortie);
    free(img->pixels);
    free(img);
}


    // 6. FILTRE MÉDIAN

int mediane(int *tab, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (tab[j] < tab[i]) {
                int tmp = tab[i];
                tab[i] = tab[j];
                tab[j] = tmp;
            }
    return tab[n / 2];
}

void fil(const char *nom, const char *sortie) {
    Image *img = chargerImage(nom);
    if (!img) return;

    Image newImg = *img;
    newImg.pixels = malloc(img->largeur * img->hauteur * sizeof(Pixel));

    int dx[9] = {-1,-1,-1,0,0,0,1,1,1};
    int dy[9] = {-1,0,1,-1,0,1,-1,0,1};

    for (int y = 1; y < img->hauteur - 1; y++) {
        for (int x = 1; x < img->largeur - 1; x++) {
            int R[9], G[9], B[9];
            for (int k = 0; k < 9; k++) {
                int nx = x + dx[k], ny = y + dy[k];
                Pixel p = img->pixels[ny * img->largeur + nx];
                R[k] = p.r; G[k] = p.g; B[k] = p.b;
            }
            newImg.pixels[y * img->largeur + x].r = mediane(R, 9);
            newImg.pixels[y * img->largeur + x].g = mediane(G, 9);
            newImg.pixels[y * img->largeur + x].b = mediane(B, 9);
        }
    }

    sauvegarderImage(&newImg, sortie);
    printf("🪄 Filtre médian appliqué : %s\n", sortie);
    free(newImg.pixels);
    free(img->pixels);
    free(img);
}

   //main
int main() {
    char ligne[256];
      // creation d'une image degrade perso
    Image* deg = malloc(sizeof(Image));

    strcpy((deg)->type,"P3");
    
    int width =300, height=300; // width=largeur, height=hauteur
    deg->largeur=width;
    deg->hauteur=height;
    deg->pixels=malloc(width*height*sizeof(Pixel));
    FILE *fo =fopen(degra,"w");
    if(!fo){
        printf("erreur de la creation du fichier");
        return 1;
    }
    fprintf(fo,"P3\n %d %d \n255\n",width,height);
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            int r=(255);// rouge si je divise r par width  et g par height je vais obtenir du une beelle image douce degrade
            int g=(255);//  vert 
            int b=100;// bleu 

            fprintf(fo,"%d %d %d ",r,g,b);
        }
        fprintf(fo,"\n");
    }
    fclose(fo);
    printf("\nl'image %s a ette creer avec succes\n",degra);
    printf("\nliste des commandes de l'application\n1- eclairer/ foncer(Usage: dom r|g|b val fichier.ppm)\n2-mettre en gris (Usage: gris fichier.ppm)\n3-taille de l'image (Usage: size fichier.ppm)\n4-couper l'image (Usage: cut fichier.ppm l1 l2 c1 c2 fichier_resultat.ppm)\n5-negation de l'image (Usage: neg fichier.ppm fichier_resultat.ppm)\n6- filtre(Usage : fil fichier.ppm fichier_resultat.ppm)\n 7-quitter\n ");
   
    while (1) {
        printf("\n \nun fichier du nom de %s associe au programme a ete creer pour vous permmettre d'executer vos commande vous pouvez vous en servir\n",degra);
      printf("saisisser la syntaxe de la commande que vous souhaiter utiliser\n");
        printf("ppmviewer> ");
        fgets(ligne, sizeof(ligne), stdin);

        char cmd[20], arg1[100], arg2[100], arg3[100], arg4[100], arg5[100];
        int v1, v2, v3, v4, val;

        int n = sscanf(ligne, "%s", cmd);
        if (n != 1) continue;

        if (strcmp(cmd, "quit") == 0) break;

        // dom c val fichier.ppm
        if (strcmp(cmd, "dom") == 0) {
            if (sscanf(ligne, "%*s %s %d %s", arg1, &val, arg2) == 3)
                dom(arg1, val, arg2);
            else printf("Usage : dom R|G|B val fichier.ppm\n");
        }
        else if (strcmp(cmd, "gris") == 0) {
            if (sscanf(ligne, "%*s %s", arg1) == 1)
                gris(arg1);
            else printf("Usage : gris fichier.ppm\n");
        }
        else if (strcmp(cmd, "size") == 0) {
            if (sscanf(ligne, "%*s %s", arg1) == 1)
                size(arg1);
            else printf("Usage : size fichier.ppm\n");
        }
        else if (strcmp(cmd, "cut") == 0) {
            if (sscanf(ligne, "%*s %s %d %d %d %d %s", arg1, &v1, &v2, &v3, &v4, arg2) == 6)
                cut(arg1, v1, v2, v3, v4, arg2);
            else printf("Usage : cut fichier.ppm l1 l2 c1 c2 fichier_resultat.ppm\n");
        }
        else if (strcmp(cmd, "neg") == 0) {
            if (sscanf(ligne, "%*s %s %s", arg1, arg2) == 2)
                neg(arg1, arg2);
            else printf("Usage : neg fichier.ppm fichier_resultat.ppm\n");
        }
        else if (strcmp(cmd, "fil") == 0) {
            if (sscanf(ligne, "%*s %s %s", arg1, arg2) == 2)
                fil(arg1, arg2);
            else printf("Usage : fil fichier.ppm fichier_resultat.ppm\n");
        }
        else {
            if(strcmp(cmd,"quitter")==0)
                 return 0;
            else printf(" Commande inconnue.\n");
        }


    }

    printf(" Fin de ppmviewer.\n");
    return 0;
}
