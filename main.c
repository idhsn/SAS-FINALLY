#include <stdio.h>
#include <stdlib.h>
#include "mery.h"

int menu(){
    const char *options[8] = {
        "Enter & Analyze Text",
        "Show Dictionary",
        "Search Exact Word",
        "Search Partial Word",
        "Sort Words",
        "Global Statistics",
        "Analyses (Palindromes, Anagrams, Word Cloud)",
        "Exit"
    };
    int choice = 0, i;
    int key;

    loading();

    while (1){
        for (i = 0; i < 8; i++){
            gotoxy(8, 8 + i);
            if (i == choice){
                textcolor(14);
                cprintf("%c %-60s", 175, options[i]);
                textcolor(7);
            } else {
                cprintf("  %-60s", options[i]);
            }
        }

        gotoxy(42, 8);
        textcolor(9);
        switch (choice){
            case 0: gotoxy(2,25);cprintf("%s", "Analyze a free text (up to 2000 chars) and build dictionary."); break;
            case 1: gotoxy(2,25);cprintf("%s", "Show all unique words with counts and positions.            "); break;
            case 2: gotoxy(2,25);cprintf("%s", "Find exact word info (case-insensitive).                    "); break;
            case 3: gotoxy(2,25);cprintf("%s", "Find words containing a substring.                          "); break;
            case 4: gotoxy(2,25);cprintf("%s", "Sort dictionary (alphabet, frequency, length).              "); break;
            case 5: gotoxy(2,25);cprintf("%s", "Show totals, averages, and extremes.                        "); break;
            case 6: gotoxy(2,25);cprintf("%s", "Palindromes, anagrams, and simple word cloud.               "); break;
            case 7: gotoxy(2,25);cprintf("%s", "Exit program.                                               "); break;
        }
        textcolor(7);

        key = getch();
        if (key == 0 || key == 224){
            int k2 = getch();
            if (k2 == 80 && choice < 7) choice++;
            else if (k2 == 72 && choice > 0) choice--;
        } else if (key == 13) {
            return choice;
        } else if (key >= '1' && key <= '8'){
            return key - '1';
        }
    }
}


int main(){
    char text[MAX_TEXT];
    char input[MAX_LEN];
    char temp[MAX_LEN];
    int choice, s, i;

    clrscr();
    draw_frame();

    while (1){
        choice = menu();

        draw_frame();

        switch (choice){
            case 0:
                gotoxy(4,5); cprintf("Enter text (max 2000 chars):");
                gotoxy(4,6);
                if (fgets(text, sizeof(text), stdin) == NULL) text[0] = '\0';
                wordcount = 0;
                analyzethetext(text);
                gotoxy(4,23); cprintf("Text analyzed successfully! Press any key to continue...");
                getch();
                break;

            case 1:
                showdictionary();
                gotoxy(4,23); cprintf("Press any key to return to menu ...");
                getch();
                break;

            case 2:
                gotoxy(4,5); cprintf("Enter word: ");
                scanf("%19s", input);
                lowering(input, temp);
                searchexact(temp);
                gotoxy(4,23); cprintf("Press any key to return to menu ...");
                getch();
                break;

            case 3:
                gotoxy(4,5); cprintf("Enter substring: ");
                scanf("%19s", input);
                lowering(input, temp);
                partialsearch(temp);
                gotoxy(4,23); cprintf("Press any key to return to menu ...");
                getch();
                break;

            case 4:
                gotoxy(4,5); cprintf("Sort by: 1-Alphabet 2-Frequency 3-Length: ");
                scanf("%d", &s);
                if (s == 1) qsort(dict, wordcount, sizeof(word), cmpAlpha);
                else if (s == 2) qsort(dict, wordcount, sizeof(word), cmpFreq);
                else if (s == 3) qsort(dict, wordcount, sizeof(word), cmpLength);
                gotoxy(4,23); cprintf("Sorted! Press any key to continue...");
                getch();
                break;

            case 5:
                globalStats();
                gotoxy(4,23); cprintf("Press any key to return to menu ...");
                getch();
                break;

            case 6:
                draw3();
                showAnalyses();
                break;

            case 7:
                gotoxy(4,23); cprintf("GOOD BYE !");
                gotoxy(1,25);return 0;
        }

        draw_frame();
    }

}
