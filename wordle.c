#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void joc();
void meniu();

// verifica cuvantul introdus si il coloreaza corespunzator
int check(char* s, char* cuvant, int nr) {
    int i, ok, j, poz, y = 1;
    cuvant[5] = '\0';
    if (strcmp(s, cuvant) == 0) {
        attron(A_INVIS);
        mvprintw(30, 20, "Cuvantul introdus este prea scurt! Continua");
        attroff(A_INVIS);
        mvprintw(30, 20, "Ati castigat!");
        for (i = 0; i < 5; i++) {
            move(4 * nr + 2, y + 1);
            attron(COLOR_PAIR(1));
            printw("%c", cuvant[i]);
            attroff(COLOR_PAIR(1));
            y = y + 4;
        }
        refresh();
        sleep(2);
        endwin();

        attron(A_INVIS);
        mvprintw(30, 20, "Cuvantul introdus este prea scurt! Continua");
        attroff(A_INVIS);
        mvprintw(30, 20, "Pentru a mai juca inca o data apasati Enter!");
        int c = getch();
        if (c == 58) {
            meniu();
        }
        if (c == 10) {
            attron(A_INVIS);
            mvprintw(30, 20, "Cuvantul introdus este prea scurt! Continua !");
            mvprintw(32, 20, "cuvan");
            attroff(A_INVIS);
            joc();
        } else {
            refresh();
            endwin();
            exit(0);
        }
    } else {
        for (i = 0; i < 5; i++) {
            move(4 * nr + 2, y + 1);
            // coloreaza cu verde
            if (s[i] == cuvant[i]) {
                attron(COLOR_PAIR(1));
                printw("%c", s[i]);
                attroff(COLOR_PAIR(1));
            } else {
                ok = 0;
                for (j = 0; j < 5; j++) {
                    if (s[j] == cuvant[i]) {
                        ok = 1;
                    }
                }
                if (ok == 1) {
                    // coloreaza cu galben
                    attron(COLOR_PAIR(2));
                    printw("%c", cuvant[i]);
                    attroff(COLOR_PAIR(2));
                } else {
                    // coloreaza cu negru
                    attron(COLOR_PAIR(3));
                    printw("%c", cuvant[i]);
                    attroff(COLOR_PAIR(3));
                }
            }
            y = y + 4;
        }
        // daca s-a ajuns la ultima incercare, cuvantul al saselea
        if (nr == 5) {
            attron(A_INVIS);
            mvprintw(30, 20, "Cuvantul introdus este prea scurt! Continua");
            attroff(A_INVIS);
            mvprintw(30, 20, "Ati pierdut! Cuvantul cautat era: ");
            move(32, 20);
            printw("%s", s);
            refresh();

            mvprintw(34, 20, "Pentru a mai juca inca o data apasati Enter!");
            int c = getch();
            if (c == 58) {
                meniu();
            }
            if (c == 10) {
                attron(A_INVIS);
                mvprintw(30, 20,
                         "Cuvantul introdus este prea scurt! Continua !");
                mvprintw(32, 20, "cuvan");
                mvprintw(34, 20,
                         "Pentru a mai juca inca o data apasati Enter!");
                attroff(A_INVIS);

                joc();
            } else {
                refresh();
                endwin();
                exit(0);
            }
        }
    }

    return 0;
}

// initializeaza ecranul principal si tabla de joc
void initializare_ecran(int* x, int* y) {
    char mesg[] = "Wordle";
    mvprintw(0, 8, "%s", mesg);

    WINDOW* win = newwin(50, 50, 0, 0);

    refresh();

    WINDOW* casuta = newwin(4, 4, 1, *y);

    refresh();

    for (int j = 0; j < 6; j++) {
        *y = 1;
        for (int i = 0; i < 5; i++) {
            casuta = newwin(3, 4, *x, *y);
            box(casuta, 0, 0);
            refresh();
            wrefresh(casuta);
            *y = *y + 4;
        }
        *x = *x + 4;
    }
}

// initializeaza dictionarul de cuvinte
void dictionar(char (*a)[15]) {
    int i;

    for (i = 0; i < 33; i++) {
        a[i][0] = '\0';
    }

    strcpy(a[0], "arici");
    strcpy(a[1], "atent");
    strcpy(a[2], "baiat");
    strcpy(a[3], "ceata");
    strcpy(a[4], "debut");
    strcpy(a[5], "peste");
    strcpy(a[6], "fixat");
    strcpy(a[7], "hamac");
    strcpy(a[8], "harta");
    strcpy(a[9], "jalon");
    strcpy(a[10], "jucam");
    strcpy(a[11], "lacat");
    strcpy(a[12], "magie");
    strcpy(a[13], "nufar");
    strcpy(a[14], "oaste");
    strcpy(a[15], "perus");
    strcpy(a[16], "rigle");
    strcpy(a[17], "roman");
    strcpy(a[18], "sanie");
    strcpy(a[19], "scris");
    strcpy(a[20], "sonda");
    strcpy(a[21], "texte");
    strcpy(a[22], "tipar");
    strcpy(a[23], "titan");
    strcpy(a[24], "zebra");
    strcpy(a[25], "vapor");
    strcpy(a[26], "vatra");
    strcpy(a[27], "color");
    strcpy(a[28], "nebun");
    strcpy(a[29], "maret");
    strcpy(a[30], "iubit");
}

// alege un numar random intre 0 si 30
int random_number() {
    int nr;
    srand(time(0));
    nr = rand() % 31;
    return nr;
}

// schimba culoarea textului cu fundalul
void print_highlighted(WINDOW* menu, int i, const char* option) {
    wattron(menu, A_REVERSE);
    mvwprintw(menu, i + 1, 50, "%s", option);
    wattroff(menu, A_REVERSE);
}

// implementeaza optiunea de meniu
void meniu() {
    const char* optiune[] = {"Iesire din aplicatie", "Reincepere joc"};
    int i;

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW* menu = newwin(6, xMax - 12, yMax - 8, 5);

    int key_pressed = -1, pointer = 0;
    box(menu, 0, 0);
    refresh();
    wrefresh(menu);
    keypad(menu, true);

    while (key_pressed != 10) {
        for (i = 0; i < 2; i++) {
            if (i == pointer) {
                print_highlighted(menu, i, optiune[i]);
            } else {
                mvwprintw(menu, i + 1, 50, "%s", optiune[i]);
            }
        }

        key_pressed = wgetch(menu);

        if (key_pressed == KEY_UP) {
            pointer--;
            if (pointer < 0) {
                pointer = 0;
            }
        }

        if (key_pressed == KEY_DOWN) {
            pointer++;
            if (pointer >= 2) {
                pointer = 2;
            }
        }
    }

    if (pointer == 1) {
        clear();
        joc();
    }

    if (pointer == 0) {
        endwin();
        exit(0);
    }
}

// apelata va genera o runda de joc
void joc() {
    int x = 1, y = 1, c;
    int i, j, nr_rand, crt;
    char a[33][15], *cuvant;

    initscr();

    start_color();
    noecho();

    cuvant = (char*)calloc(15, sizeof(char));

    initializare_ecran(&x, &y);

    dictionar(a);
    nr_rand = random_number();

    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(2, COLOR_WHITE, COLOR_YELLOW);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    x = y = 1;
    i = 0;
    j = 0;
    crt = 0;

    while (j < 6) {
        i = 0;
        y = 1;
        while (i >= 0 && i < 5) {
            move(x + 1, y + 1);
            c = getch();
            if (c == 58) {
                meniu();
            }
            if (c == 10) {
                mvprintw(30, 20, "Cuvantul introdus este prea scurt! Continua");
            }
            if (isalpha(c) != 0) {
                printw("%c", c);
                cuvant[crt] = c;
                crt++;
                y = y + 4;
                i++;
            } else if (c == 127 && i != 0) {
                move(x - 1, y - 1);
                y = y - 4;
                crt--;
                i--;
            }
        }
        
        attron(A_INVIS);
        c = getch();
        attroff(A_INVIS);

        if (c == 58) {
            meniu();
        }
        if (c == 127) {
            move(x - 1, y - 1);
            i--;
            crt--;
            y = y - 4;
            while (i >= 0 && i < 5) {
                move(x + 1, y + 1);
                c = getch();
                if (c == 58) {
                    meniu();
                }
                if (isalpha(c) != 0) {
                    printw("%c", c);
                    cuvant[crt] = c;
                    y = y + 4;
                    i++;
                    crt++;
                } else if (c == 127 && i != 0) {
                    move(x - 1, y - 1);
                    i--;
                    y = y - 4;
                    crt--;
                }
            }
            c = getch();
        }
        if (c == 58) {
            meniu();
        }
        if (c == 10) {
            attron(A_INVIS);
            mvprintw(30, 20, "Cuvantul introdus este prea scurt! Continua");
            attroff(A_INVIS);
            mvprintw(30, 20, "Cuvantul este verificat!");
            check(a[nr_rand], cuvant, j);
            x = x + 4;
            j++;
            crt = 0;
        } else {
            while (c != 10) {
                attron(A_INVIS);
                mvprintw(30, 20, "Cuvantul introdus este prea scurt! Continua");
                attroff(A_INVIS);
                refresh();
                mvprintw(30, 20, "Apasa tasta enter pentru a verifica!");
                c = getch();
            }
            attron(A_INVIS);
            mvprintw(30, 20, "Cuvantul introdus este prea scurt! Continua");
            attroff(A_INVIS);
            mvprintw(30, 20, "Cuvantul este verificat!");
            check(a[nr_rand], cuvant, j);
            x = x + 4;
            j++;
            crt = 0;
        }
    }

    getch();
    refresh();
    endwin();
}

int main() {
    joc();
    return 0;
}
