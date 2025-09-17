#ifndef MERY_H
#define MERY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define MAX_LEN 20
#define MAX_TEXT 2000
#define MAX_POS 200
#define MAX_WORDS 500

// prototypes
void lowering(char *src,char *dest);
void addword (char *added, int position);
void analyzethetext(char *text);
void showdictionary(void);
void searchexact(const char *searched);
void partialsearch(const char *partialS);
void globalStats(void);
void draw3(void);
void draw_frame(void);
void showAnalyses(void);
int cmpAlpha(const void *a, const void *b);
int cmpLength(const void *a, const void *b);
int cmpFreq(const void *a, const void *b);
int compareChars(const void *a, const void *b);
int menu(void);


// draw the frame
void draw_frame() {
    int i;

    system("cls");

    for (i = 2; i <= 78; i++) {
        gotoxy(i, 1);  printf("%c", 205); // ─
        gotoxy(i, 3);  printf("%c", 205);
        gotoxy(i, 22); printf("%c", 205);
        gotoxy(i, 24); printf("%c", 205);
    }

    for (i = 2; i <= 24; i++) {
        gotoxy(1, i);  printf("%c", 186); // │
        gotoxy(78, i); printf("%c", 186);
    }

    gotoxy(1,1);   printf("%c", 201); // ╔
    gotoxy(78,1);  printf("%c", 187); // ╗
    gotoxy(1,3);   printf("%c", 204); // ╠
    gotoxy(78,3);  printf("%c", 185); // ╣
    gotoxy(1,22);  printf("%c", 204); // ╠
    gotoxy(78,22); printf("%c", 185); // ╣
    gotoxy(1,24);  printf("%c", 200); // ╚
    gotoxy(78,24); printf("%c", 188); // ╝

    textcolor(11);
    gotoxy(4,2); cprintf(" Mini Search Engine ");
    textcolor(7);
}

//draw the 3 lines for the analayses
void draw3() {
    int i;

    gotoxy(25,22); cprintf("%c",202);
    gotoxy(52,22); cprintf("%c",202);
    gotoxy(25,3); cprintf("%c",203);
    gotoxy(52,3); cprintf("%c",203);

    for (i = 4; i < 22; i++){
        gotoxy(25,i);printf("%c",186);
        gotoxy(52,i);printf("%c",186);
    }
};

// the main structre
typedef struct {
    char word[MAX_LEN];
    int count, length, position[MAX_POS], poscount;
} word;

word dict[MAX_WORDS];
int wordcount = 0;

// to clean the text and set
void lowering(char *src,char *dest){
    int i, j;
    i = j = 0;
    while (src[i]){
        if(isalpha(src[i])){
            dest[j] = tolower(src[i]);
            j++;
        }
        i++;
    }
    dest[j] = '\0';
}

// adding a word
void addword (char *added, int position){
    int i;
    for ( i = 0 ; i < wordcount ; i++){
        if (strcmp(dict[i].word, added) == 0){
            dict[i].count++;
            dict[i].position[dict[i].poscount++] = position;
            return;
        }
    }

    strcpy(dict[wordcount].word, added);
    dict[wordcount].count = 1;
    dict[wordcount].length = strlen(added);
    dict[wordcount].position[0] = position;
    dict[wordcount].poscount = 1;
    wordcount++;
}

// seperates the texts to words
void analyzethetext (char *text){
    char *token, lowered[MAX_LEN];
    int pos = 1;

    token = strtok(text," \t\n");

    while (token != NULL){
        lowering(token, lowered);
        addword(lowered, pos++);
        token = strtok(NULL, " \t\n");
    }
}
// show the dictionary
void showdictionary(){
    int i, j, y = 5;
    for (i = 0; i < wordcount; i++){
        gotoxy(4, y);
        printf("%-15s | count = %3d | length = %2d | positions = ", dict[i].word, dict[i].count, dict[i].length);
        for (j = 0; j < dict[i].poscount; j++) printf("%d ", dict[i].position[j]);
        printf("\n");
        y++;
        if (y > 21) {
            gotoxy(4,23); cprintf("Press any key to return to menu ...");
            getch();
            draw_frame();
            y = 5;
        }
    }
}

// search for an exact word
void searchexact(const char *searched){
    int i, j;
    for (i = 0; i < wordcount; i++){
        if (strcmp(dict[i].word, searched) == 0){
            gotoxy(4,5);printf("word = %s | count = %d | length = %d | positions = ", dict[i].word, dict[i].count, dict[i].length);
            for (j = 0; j < dict[i].poscount; j++) printf("%d ", dict[i].position[j]);
            return;
        }
    }
    gotoxy(4,5);printf("Word was not found!\n");
}

//search for a partial word like he in the or something
void partialsearch(const char *partialS){
    int i, y = 5;
    int found = 0;
    for (i = 0; i < wordcount; i++){
        if (strstr(dict[i].word, partialS) != NULL){
            gotoxy(4,y);printf("%s | count = %d | length = %d\n", dict[i].word, dict[i].count, dict[i].length);
            y++;
            found = 1;
        }
    }
    if (!found) {
        gotoxy(4,5);printf("No word found containing '%s'", partialS);
    }
}

// sorting logics that works with qsort
int cmpAlpha(const void *a, const void *b){
    const word *w1 = (const word *)a;
    const word *w2 = (const word *)b;
    return strcmp(w1->word, w2->word);
}

int cmpLength(const void *a, const void *b){
    const word *w1 = (const word *)a;
    const word *w2 = (const word *)b;
    return w1->length - w2->length;
}

int cmpFreq(const void *a, const void *b){
    const word *w1 = (const word *)a;
    const word *w2 = (const word *)b;
    return w2->count - w1->count;
}

// all the statics
void globalStats(){
    int i;
    int total = 0, totalwords = 0, maxLen = 0, minLen = MAX_LEN, maxFreq = 0;
    const char *longest = NULL, *shortest = NULL, *mostFreq = NULL;

    if (wordcount == 0){
        printf("No words analyzed.\n");
        return;
    }

    for (i = 0; i < wordcount; i++){
        total += dict[i].length * dict[i].count;
        totalwords += dict[i].count;
        if (dict[i].length > maxLen){ maxLen = dict[i].length; longest = dict[i].word; }
        if (dict[i].length < minLen){ minLen = dict[i].length; shortest = dict[i].word; }
        if (dict[i].count > maxFreq){ maxFreq = dict[i].count; mostFreq = dict[i].word; }
    }

    gotoxy(5,5);printf("Total words : %d ", totalwords);
    gotoxy(5,6);printf("unique words : %d ", wordcount);
    gotoxy(5,7);printf("Average Length : %d ", total / totalwords);
    gotoxy(5,8);printf("shortest word : %s ", shortest);
    gotoxy(5,9);printf("longest word : %s ", longest);
    gotoxy(5,10);printf("Most frequent: %s (%d)", mostFreq, maxFreq);
}

// recheck if the logic works
int compareChars(const void *a, const void *b){
    return (*(char*)a - *(char*)b);
}

void showAnalyses() {
    int i, j, y, k;
    char a[MAX_LEN], b[MAX_LEN];

    // === Word Cloud ===
    y = 4;
    for (i = 0; i < wordcount; i++) {
        gotoxy(4, y);
        printf("%-10s: ", dict[i].word);
        for (j = 0; j < dict[i].count && j < 20; j++) printf("*"); // cap stars to 20
        y++;
        if (y > 21) { // pause & reset
            gotoxy(4, 23); cprintf("Press any key to continue ..."); getch();
            for ( k = 4; k <= 21; k++) { gotoxy(4, k); printf("%s", "                     "); }
            y = 4;
        }
    }

    // === Palindromes ===
    y = 4;
    for (i = 0; i < wordcount; i++) {
        int len = dict[i].length;
        int flag = 1;
        for (j = 0; j < len / 2; j++) if (dict[i].word[j] != dict[i].word[len - 1 - j]) { flag = 0; break; }
        if (flag) {
            gotoxy(28, y);
            printf("%-20s", dict[i].word);
            y++;
            if (y > 21) {
                gotoxy(4, 23); cprintf("Press any key to continue ..."); getch();
                for ( k = 4; k <= 21; k++) { gotoxy(28, k); printf("%-20s", "                    "); }
                y = 4;
            }
        }
    }

    // === Anagrams ===
    y = 4;
    for (i = 0; i < wordcount; i++) {
        for (j = i + 1; j < wordcount; j++) {
            if (dict[i].length != dict[j].length) continue;
            strcpy(a, dict[i].word);
            strcpy(b, dict[j].word);
            qsort(a, dict[i].length, sizeof(char), compareChars);
            qsort(b, dict[j].length, sizeof(char), compareChars);
            if (strcmp(a, b) == 0) {
                gotoxy(54, y);
                printf("%s <=> %s", dict[i].word, dict[j].word);
                y++;
                if (y > 21) {
                    gotoxy(4, 23); cprintf("Press any key to continue ..."); getch();
                    for ( k = 4; k <= 21; k++) { gotoxy(54, k); printf("%-25s", "                    "); }
                    y = 4;
                }
            }
        }
    }

    // Final pause for partially filled columns
    gotoxy(4, 23); cprintf("Press any key to return to menu ...");
    getch();
}
#endif

