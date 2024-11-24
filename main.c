#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define TAILLE_MIN 6
#define TAILLE_MAX 20
#define NOMBRE_MOTS 4
#define MAX_THEMES 10
#define MAX_THEME_NAME 50

char *themes[] = {
    "Animaux", "Pays", "Fruits", "Couleurs"
};

// Listes de mots par thème
char *animaux[] = {"requin", "chien", "castor", "elan", "chat", "lapin", "okapi", "lion", "oiseau", "rat", "taupe", "tortue", "yack", "ane", "vache", "mule", NULL};
char *pays[] = {"albanie", "bahamas", "chili", "danemark", "finlande", "georgie", "haiti", "inde", "japon", "kenya", "lituanie", "mexique", "norvege", "perou", "roumanie", "senegal", "thailande", NULL};
char *fruits[] = {"pomme", "banane", "orange", "kiwi", "mangue", "fraise", "raisin", "peche", "cerise", "citron", NULL};
char *couleurs[] = {"rouge", "bleu", "vert", "jaune", "noir", "blanc", "rose", "violet", "marron", "gris", NULL};

char **mots; // Declare mots in the global scope
int nbMots = 0;

char *userThemes[MAX_THEMES][NOMBRE_MOTS];
char userThemeNames[MAX_THEMES][MAX_THEME_NAME];
int userThemeCount = 0;

char grille[TAILLE_MAX][TAILLE_MAX];
int couleurCase[TAILLE_MAX][TAILLE_MAX];

int posX = 0;
int posY = 0;
int selectionActive = 0;
int startX = -1, startY = -1;  // Initialisation à -1 pour indiquer aucune sélection

int taille = 10;

void choisir_theme() {
    int choix_theme;
    system("cls");
    printf("Choisissez un thème (theme actuel: %s):\n", mots);
    for (int i = 0; i < sizeof(themes) / sizeof(themes[0]); i++) {
        printf("%d. %s\n", i + 1, themes[i]);
    }
    scanf("%d", &choix_theme);

    // Update mots and nbMots based on the chosen theme
    switch(choix_theme) {
        case 1:
            mots = animaux;
            break;
        case 2:
            mots = pays;
            break;
        case 3:
            mots = fruits;
            break;
        case 4:
            mots = couleurs;
            break;
        default:
            printf("Choix invalide. Thème par défaut (Animaux) sélectionné.\n");
            mots = animaux;
    }

    // Calculate nbMots based on the chosen theme
    nbMots = 0;
    while (mots[nbMots] != NULL) {
        nbMots++;
    }
}

void setColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (textColor & 0x0F) | (bgColor << 4));
}

void resetColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (15 & 0x0F) | (0 << 4)); // white text on black background
}

int canPlaceWord(int startX, int startY, int len, int direction) {
    int stepX = 0, stepY = 0;
    if (direction == 0) { // horizontal
        stepX = 1;
    } else if (direction == 1) { // vertical
        stepY = 1;
    } else if (direction == 2) { // diagonal
        stepX = 1;
        stepY = 1;
    }

    for (int i = 0; i < len; i++) {
        if (grille[startY + i * stepY][startX + i * stepX] != ' ') {
            return 0;
        }
    }
    return 1;
}

void initialiserGrille(int taille, int nbMots, char *mots[]) {
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            grille[i][j] = ' '; // Initialize all cells to empty
        }
    }

    int indices[NOMBRE_MOTS];
    for (int i = 0; i < NOMBRE_MOTS; i++) {
        while (1) {
            int idx = rand() % nbMots;
            int j;
            for (j = 0; j < i; j++)
                if (indices[j] == idx)
                    break;
            if (j == i) {
                indices[i] = idx;
                break;
            }
        }
    }

    for (int k = 0; k < NOMBRE_MOTS; k++) {
        int len = strlen(mots[indices[k]]);
        int placed = 0;

        while (!placed) {
            int direction = rand() % 3; // 0: horizontal, 1: vertical, 2: diagonal
            int startX = rand() % (taille - len + 1);
            int startY = rand() % (taille - len + 1);

            if (canPlaceWord(startX, startY, len, direction)) {
                if (direction == 0) { // horizontal
                    for (int i = 0; i < len; i++) {
                        grille[startY][startX + i] = mots[indices[k]][i];
                    }
                } else if (direction == 1) { // vertical
                    for (int i = 0; i < len; i++) {
                        grille[startY + i][startX] = mots[indices[k]][i];
                    }
                } else if (direction == 2) { // diagonal
                    for (int i = 0; i < len; i++) {
                        grille[startY + i][startX + i] = mots[indices[k]][i];
                    }
                }
                placed = 1;
            }
        }
    }

    // Fill the remaining cells with random letters
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            if (grille[i][j] == ' ') {
                grille[i][j] = 'A' + rand() % 26;
            }
        }
    }
}



void afficherGrille(int taille) {
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            if (i == posY && j == posX) {
                setColor(15, 5); // white text on magenta background
                printf("[%c] ", grille[i][j]);
            } else if (i == startY && j == startX) {
                setColor(0, 14); // black text on yellow background
                printf("[%c] ", grille[i][j]);
            } else if (couleurCase[i][j] == 1) {
                setColor(15, 2); // white text on green background
                printf("[%c] ", grille[i][j]);
            } else {
                if ((i + j) % 2 == 0) {
                    setColor(0, 7); // black text on light gray background
                } else {
                    setColor(15, 0); // white text on black background
                }
                printf("[%c] ", grille[i][j]);
            }
            resetColor();
        }
        printf("\n");
    }
}

int verifierMot(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int len = abs(dx) > abs(dy) ? abs(dx) + 1 : abs(dy) + 1;
    char mot[TAILLE_MAX];
    int index = 0;

    // Check if the selected coordinates form a straight line (horizontal, vertical, or diagonal)
    if (abs(dx) != abs(dy) && dx != 0 && dy != 0) {
        return 0;
    }

    int stepX = dx == 0 ? 0 : (dx > 0 ? 1 : -1);
    int stepY = dy == 0 ? 0 : (dy > 0 ? 1 : -1);

    for (int i = 0; i < len; i++) {
        mot[i] = grille[y1 + i * stepY][x1 + i * stepX];
    }
    mot[len] = '\0';

    for (int i = 0; mots[i] != NULL; i++) {
        if (strcmp(mot, mots[i]) == 0) {
            for (int j = 0; j < len; j++) {
                couleurCase[y1 + j * stepY][x1 + j * stepX] = 1;
            }
            return 1;
        }
    }
    return 0;
}

void afficherListeMots(char *mots[], int nbMots, const char* titre) {
    printf("%s:\n", titre);
    for (int i = 0; i < nbMots; i++) {
        printf("%s\n", mots[i]);
    }
    printf("\n");
}

void ajouterTheme() {
    if (userThemeCount >= MAX_THEMES) {
        printf("Nombre maximum de thèmes atteint.\n");
        return;
    }

    char themeName[MAX_THEME_NAME];
    char themeMots[NOMBRE_MOTS][TAILLE_MAX];

    printf("Entrez le nom du nouveau thème : ");
    scanf("%s", themeName);
    strcpy(userThemeNames[userThemeCount], themeName);

    for (int i = 0; i < NOMBRE_MOTS; i++) {
        printf("Entrez le mot %d du thème : ", i + 1);
        scanf("%s", themeMots[i]);
        userThemes[userThemeCount][i] = (char *)malloc(TAILLE_MAX * sizeof(char));
        strcpy(userThemes[userThemeCount][i], themeMots[i]);
    }

    userThemeCount++;
    printf("Thème ajouté avec succès.\n");

    // Initialise la grille avec le nouveau thème et lance le jeu
    initialiserGrille(taille, NOMBRE_MOTS, (char**)themeMots);
    jouerJeu((char**)themeMots, NOMBRE_MOTS);
}

void jouerJeu(char *mots[], int nbMots) {
    char motsTrouves[NOMBRE_MOTS][TAILLE_MAX];
    int nbMotsTrouves = 0;
    char motsRestants[NOMBRE_MOTS][TAILLE_MAX];
    int nbMotsRestants = nbMots;

    // Initialize motsRestants
    for (int i = 0; i < nbMots; i++) {
        strcpy(motsRestants[i], mots[i]);
    }

    while (nbMotsTrouves < nbMots) {
        system("cls");
        afficherGrille(taille);

        // Display word lists
        printf("\n");
        afficherListeMots((char**)motsRestants, nbMotsRestants, "Mots à trouver");
        afficherListeMots((char**)motsTrouves, nbMotsTrouves, "Mots trouvés");

        char touche = getch();

        switch (touche) {
            case '2': // bas
                posY = (posY + 1) % taille;
                break;
            case '4': // gauche
                posX = (posX - 1 + taille) % taille;
                break;
            case '6': // droite
                posX = (posX + 1) % taille;
                break;
            case '8': // haut
                posY = (posY - 1 + taille) % taille;
                break;
            case 'q': // quitter
                return;
            case ' ': // sélectionner la lettre
                if (!selectionActive) {
                    selectionActive = 1;
                    startX = posX;
                    startY = posY;
                } else {
                    selectionActive = 0;
                    if (verifierMot(startX, startY, posX, posY)) {
                        // Move the found word from motsRestants to motsTrouves
                        for (int k = 0; k < nbMotsRestants; k++) {
                            if (strcmp(motsRestants[k], mots[k]) == 0) {
                                strcpy(motsTrouves[nbMotsTrouves], motsRestants[k]);
                                nbMotsTrouves++;
                                for (int j = k; j < nbMotsRestants - 1; j++) {
                                    strcpy(motsRestants[j], motsRestants[j + 1]);
                                }
                                nbMotsRestants--;
                                break;
                            }
                        }
                    }
                    startX = -1; // Reset selection start after word is found
                    startY = -1; // Reset selection start after word is found
                }
                break;
        }
    }
}

void menuPrincipal() {
    int choix;
    while (1) {
        system("cls");
        printf("Choisissez une option:\n");
        printf("1. Jouer au jeu\n");
        printf("2. Changer de theme\n");
        printf("3. Modifier la taille de la grille\n");
        printf("4. Créer un nouveau thème\n");
        printf("5. Quitter\n");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                // Play the game
                initialiserGrille(taille, nbMots, mots);
                jouerJeu(mots, nbMots);
                return; // Exit the menu to start the game

            case 2:
                // Choose a new theme
                choisir_theme();
                break;

            case 3:
                // Change grid size
                system("cls");
                do {
                    printf("Entrez la taille de la grille (minimun %d, maximum %d) : ", TAILLE_MIN, TAILLE_MAX);
                    scanf("%d", &taille);
                    if (taille < TAILLE_MIN || taille > TAILLE_MAX) {
                        printf("Taille invalide. Utilisez une taille entre %d et %d.\n", TAILLE_MIN, TAILLE_MAX);
                    }
                } while (taille < TAILLE_MIN || taille > TAILLE_MAX);
                break;

            case 4:
                // Create a new theme
                ajouterTheme();
                break;

            case 5:
                // Quit
                exit(0);

            default:
                printf("Option invalide. Choisissez un nombre entre 1 et 5.\n");
                break;
        }
    }
}

int main() {
    srand(time(NULL));
    mots = animaux; // Initialize mots to one of the themes
    nbMots = 0;
    while (animaux[nbMots] != NULL) {
        nbMots++;
    }
    menuPrincipal();
    return 0;
}
