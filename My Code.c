#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_LEN 20
#define MAX_TEXT 2000
#define MAX_POS 200
#define MAX_WORDS 500

//prototypes.
void lowering(char *src, char *dest);
void addword(char *added, int position);
void analyzethetext(char *text);
void showdictionary(void);
void searchexact(char *searched);
void partialsearch(char *partialS);
void globalStats(void);
void showpalindromes(void);
void showAnagrams(void);
void wordCloud(void);


typedef struct {
    char word[MAX_LEN];
    int count, length, position[MAX_POS], poscount;
}word;

word dict[MAX_WORDS];
    int wordcount = 0;

void lowering(char *src,char *dest){
    int j = 0, i;
    for ( i = 0; src[i]; i++){
        if (isalpha(src[i]))dest[j] = (tolower(src[i]));
        //dest[j] = '\0';
    }
}

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

void showdictionary() {
    int i, j;

    for (i = 0; i < wordcount; i++){
        printf("%s | count = %d | length = %d | ", dict[i].word, dict[i].count, dict[i].length);
    for (j = 0; j < dict[i].poscount; j++){
        printf("count = %d \n", dict[i].position[j]);
        }
    }
}

void searchexact (char *searched){
    int i, j;
    for (i = 0; i < wordcount; i++){
        if(strcmp(dict[i].word, searched) == 0){
            printf("word = %s | count = %s | length = %d | ", dict[i].word, dict[i].count, dict[i].length);
        for (j = 0; j < dict[i].poscount; j++){
        printf("count = %d \n", dict[i].position[j]);
            }
        }
    }
    printf("word was not found !");
}

void partialsearch (char *partialS){
    int i;
    for (i = 0; i < wordcount; i++){
        if (strstr(dict[i].word, partialS) != NULL){
            printf("%s | count = %d | Length = %d", dict[i].word, dict[i].count, dict[i].length);
        }else{
            printf("No word found containing %s", partialS);
        }
    }
}

//==================sorting=============================

int cmpAlpha(const void *a, const void *b) {
    return strcmp(((word*)a)->word, ((word*)b)->word);
}
int cmpFreq(const void *a, const void *b) {
    return ((word*)b)->count - ((word*)a)->count;
}
int cmpLength(const void *a, const void *b) {
    return ((word*)a)->length - ((word*)b)->length;
}

//=================== (yet to be understood) done

void globalStats() {
    int total = 0, maxLen = 0, minLen = 20, maxFreq = 0, totalwords = 0, i;
    char *longest = NULL, *shortest = NULL, *mostFreq = NULL;

    if (wordcount == 0){
        printf("No words analyzed. \n");
    return;
    }
    for (i = 0; i < wordcount; i++){
        total = total + dict[i].length * dict[i].count;

        if (dict[i].length > maxLen){
            maxLen = dict[i].length;
            longest = dict[i].word;
        }

        if (dict[i].length < minLen){
            minLen = dict[i].length;
            shortest = dict[i].word;
        }

        if (dict[i].count > maxFreq){
            maxFreq = dict[i].count;
            mostFreq = dict[i].word;
        }

        totalwords = totalwords + dict[i].count;
    }

    printf("Total words : %d \n", totalwords);
    printf("unique words : %d \n", wordcount);
    printf("Average Length : %d \n", total / totalwords);
    printf("shortest word : %d \n", shortest);
    printf("longest word : %d \n", longest);
    printf("Most frequent: %s (%d)\n", mostFreq, maxFreq);
}

void showpalindromes(){
    int i, j, len = dict[i].length, flag = 1;
    char temp;
    for( i = 0; i < wordcount; i++){
        for (j = 0; j < len / 2; j++){
            temp = dict[i].word[len - 1 - j];
            if (dict[i].word[j] != temp){
                flag = 0;
                break;
            }
        }
        if (flag){
            printf("%s\n", dict[i]);
        }
    }
}

//YET TO BE UNDERSTOOD

int compareChars(const void *a, const void *b){
    return (*(char*)a - *(char*)b);
}


// from here understand whats going on.

void showAnagrams() {
    int i, j;
    char a[MAX_LEN], b[MAX_LEN];
    for (i = 0; i < wordcount; i++) {
        for (j = i+1; j < wordcount; j++) {
            if (dict[i].length != dict[j].length) continue;
            strcpy(a, dict[i].word);
            strcpy(b, dict[j].word);
            qsort(a, strlen(a), sizeof(char), compareChars);
            qsort(b, strlen(b), sizeof(char), compareChars);
            if (strcmp(a, b) == 0)
                printf("%s <=> %s\n", dict[i].word, dict[j].word);
        }
    }
}

// Word cloud
void wordCloud() {
    int i, j;
    for ( i = 0; i < wordcount; i++) {
        printf("%s: ", dict[i].word);
        for ( j = 0; j < dict[i].count; j++) printf("*");
        printf("\n");
    }
}


int main() {
    char text[MAX_TEXT], input[MAX_LEN];
    int choice, s;

    while (1) {
        printf("\n===== Mini Search Engine =====\n");
        printf("1. Enter and analyze text\n");
        printf("2. Show dictionary\n");
        printf("3. Search exact word\n");
        printf("4. Search partial word\n");
        printf("5. Sort words\n");
        printf("6. Global statistics\n");
        printf("7. Analyses (palindromes, anagrams, word cloud)\n");
        printf("8. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar(); // remove newline

        switch(choice) {
            case 1:
                printf("Enter text (max 2000 chars):\n");
                fgets(text, MAX_TEXT, stdin);
                wordcount = 0;
                analyzeText(text);
                printf("Text analyzed successfully!\n");
                break;
            case 2: showDictionary(); break;
            case 3:
                printf("Enter word: ");
                scanf("%s", input);
                cleanWord(input, input);
                searchExact(input);
                break;
            case 4:
                printf("Enter substring: ");
                scanf("%s", input);
                cleanWord(input, input);
                searchPartial(input);
                break;
            case 5:
                printf("Sort by: 1-Alphabet 2-Frequency 3-Length: ");
                scanf("%d", &s);
                if (s==1) qsort(dict, wordcount, sizeof(word), cmpAlpha);
                else if (s==2) qsort(dict, wordcount, sizeof(word), cmpFreq);
                else if (s==3) qsort(dict, wordcount, sizeof(word), cmpLength);
                printf("Sorted!\n");
                break;
            case 6: globalStats(); break;
            case 7:
                printf("Palindromes:\n"); showPalindromes();
                printf("Anagrams:\n"); showAnagrams();
                printf("Word Cloud:\n"); wordCloud();
                break;
            case 8: exit(0);
        }
    }
}
