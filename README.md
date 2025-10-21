# Projet de Manipulation d’Images PPM

# Description
Ce projet a pour objectif de manipuler des images au format *PPM (Portable PixMap)* à travers différentes opérations de traitement d’image.  
Pendant la realisation de ce tp, chaque membre de du groupe a realise une tâche spécifique selon cette repatition

# Répartition des tâches

-Tâche 1 – Ajustement de la luminosité des pixels  
Réalise par **BENGON À MENTONG ZACK CAREL**  

 Description :
Cette partie consiste à **foncer ou éclaircir** les pixels d’une image selon leur **dominante de couleur** (rouge, verte ou bleue).  
- La **dominante** d’un pixel est la couleur (rouge, vert ou bleu) qui a la valeur la plus élevée.  
- Pour **foncer**, il faut **ajouter** un petit nombre à chacune des trois composantes du pixel.  
- Pour **éclaircir**, il faut **retirer** ce même nombre.  

Cela permet de rendre certaines zones de l’image plus sombres ou plus claires selon le besoin.

---

-Tâche 2 – Passage en noir et blanc (niveaux de gris)  
Réalise par **KOUYIM AMINATOU IRINA**  

 Description :
Cette tâche consiste à transformer une image couleur en **noir et blanc**.  
Pour cela, on calcule la **moyenne** des trois couleurs (rouge, vert, bleu) de chaque pixel.  
Cette moyenne est ensuite donnée aux trois composantes du pixel.  
Ainsi, tous les pixels auront des valeurs similaires, ce qui donne une image en **niveaux de gris**.

---

 Tâche 3 – Création du négatif de l’image  
Réalise par  **WANDJI NJAMPOU LYNN CÂLINE**  

 Description : 
Ici, le but est de créer le **négatif** de l’image.  
Pour chaque couleur du pixel (R, G, B), on calcule la **valeur opposée** à partir de 255.  
La nouvelle valeur devient :  
> 255 - valeur d’origine  

Par exemple, si un pixel a une valeur rouge de 100, la nouvelle valeur sera 155.  
Le résultat final donne une image avec les **couleurs inversées**, comme un film photo négatif.

---

Tâche 4 – Affichage de la taille de l’image  
Réalise par : **MINYEMECK BAMA BERNARD ESTIN**  

 Description :
Cette tâche consiste à **afficher la taille** de l’image en pixels.  
Le programme doit lire dans le fichier PPM les **dimensions** suivantes :  
- la **largeur** (nombre de colonnes),  
- la **hauteur** (nombre de lignes).  

Le résultat affichera quelque chose comme :  
> Taille de l’image : 640 × 480 pixels  

C’est utile pour connaître les dimensions exactes de l’image avant de la modifier.

---

Tâche 5 – Découpage d’une partie d’image  
Réalise par **TCHO PIERRE ANGE**  

 Description :
Cette partie permet de **découper une zone précise** de l’image.  
On choisit deux lignes (`l1`, `l2`) et deux colonnes (`c1`, `c2`) qui définissent la portion à garder.  
Le programme extrait alors uniquement cette partie et l’enregistre dans un **nouveau fichier PPM**.  

C’est pratique pour **recadrer** une image ou isoler une zone importante.

---

Tâche 6 – Application d’un filtre médian  
Réalise par : **KUMBAN MARC VIVIEN LONDONBELL**  

Description :
Le filtre médian sert à **améliorer la qualité de l’image** en supprimant le **bruit** (petits points ou pixels anormaux).  
Pour chaque pixel, on prend les valeurs de ses **voisins proches** et on calcule la **valeur médiane** de chaque couleur (rouge, vert, bleu).  
Le pixel est ensuite remplacé par ces nouvelles valeurs.  
Cela rend l’image plus **lisse et propre**, sans la rendre floue.

---

##  Langage utilisé
- **C**

## Format d’image
- **PPM (Portable PixMap)**

## Objectif global
Apprendre à manipuler des fichiers image et à réaliser des traitements simples en **langage C** :
- lecture et écriture d’images PPM,  
- traitement pixel par pixel,  
- utilisation de boucles et conditions.  

Ce projet aide à mieux comprendre le fonctionnement des **images numériques** et la **logique du traitement d’image**.

## Organisation du devoir

├── src/ → contient le code source et le Makefile
│ ├── main.c
│ ├── ppm_app.c
│ ├── ppm_app.h
│ └── makefile
└── README.md

## Règles de Compilation et d'éxécution


