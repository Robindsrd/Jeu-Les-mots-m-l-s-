#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>

#define TAILLE_GRILLE 10

#define NOMBRE_MOTS 1

#define NOMBRE_DE_COULEURS 5

const char* mots[NOMBRE_MOTS] = {
    "henry",
    "nico",
    "roro",
    "gabriel",

};


char grille[TAILLE_GRILLE][TAILLE_GRILLE];
int couleur[TAILLE_GRILLE][TAILLE_GRILLE];


int posX = 0;
int posY = 0;


void initialiserGrille() {
    for (int i = 0; i < TAILLE_GRILLE; i++) {
        for (int j = 0; j < TAILLE_GRILLE; j++) {
            grille[i][j] = 'A' + rand() % 26;
            couleur[i][j] = 0;
        }
    }


    for (int k = 0; k < NOMBRE_MOTS; k++) {
        int len = strlen(mots[k]);
        int direction = rand() % 4;

        int startX, startY;
        if (direction == 0) {
            startX = rand() % (TAILLE_GRILLE - len);
            startY = rand() % TAILLE_GRILLE;
        } else if (direction == 1) {
            startX = rand() % TAILLE_GRILLE;
            startY = rand() % (TAILLE_GRILLE - len);
        } else if (direction == 2) {
            startX = rand() % (TAILLE_GRILLE - len);
            startY = rand() % (TAILLE_GRILLE - len);
        } else {
            startX = rand() % (TAILLE_GRILLE - len) + len;
            startY = rand() % (TAILLE_GRILLE - len);
        }

        for (int i = 0; i < len; i++) {
            grille[startY][startX] = mots[k][i];
            if (direction == 0) {
                startX++;
            } else if (direction == 1) {
                startY++;
            } else if (direction == 2) {
                startX++;
                startY++;
            } else {
                startX--;
                startY++;
            }
        }
    }
}


void afficherGrille() {
    for (int i = 0; i < TAILLE_GRILLE; i++) {
        for (int j = 0; j < TAILLE_GRILLE; j++) {
            if (i == posY && j == posX) {
                printf("\033[1;37;41m[%c]\033[0m ", grille[i][j]);
            } else {
                printf("[%c] ", grille[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));


    initialiserGrille();


    while (1) {
        system("cls");

        afficherGrille();

        char touche = getch();

        switch (touche) {
            case '2': // Bas
                posY = (posY + 1) % TAILLE_GRILLE;
                break;
            case '4': // Gauche
                posX = (posX - 1 + TAILLE_GRILLE) % TAILLE_GRILLE;
                break;
            case '6': // Droite
                posX = (posX + 1) % TAILLE_GRILLE;
                break;
            case '8': // Haut
                posY = (posY - 1 + TAILLE_GRILLE) % TAILLE_GRILLE;
                break;
            case ' ': // Espace
                couleur[posY][posX] = (couleur[posY][posX] + 1) % NOMBRE_DE_COULEURS;
                break;
            case 'q' : //quitter
                return 0;
        }
    }
    return 0;
}
