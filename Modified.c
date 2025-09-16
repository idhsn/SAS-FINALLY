#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 20
#define MAX_TEXT 2000
#define MAX_POS 200
#define MAX_WORDS 500

typedef struct {
    char word[MAX_LEN];
    int count;
    int length;
    int positions[MAX_POS];
    int poscount;
} word;

word dict[MAX_WORDS];
int wordcount = 0;

// Function prototypes
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

// Convert to lowercase and remove punctuation
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

// Add a word to dictionary
void addword(char *added, int position){
    int i;
    i = 0;
    if(strlen(added) == 0) return;

    while(i < wordcount){
        if(strcmp(dict[i].word, added) == 0){
            dict[i].count++;
            dict[i].positions[dict[i].poscount++] = position;
            return;
        }
        i++;
    }

    strcpy(dict[wordcount].word, added);
    dict[wordcount].count = 1;
    dict[wordcount].length = strlen(added);
    dict[wordcount].positions[0] = position;
    dict[wordcount].poscount = 1;
    wordcount++;
}

// Analyze the text
void analyzethetext(char *text){
    char lowered[MAX_LEN];
    char *token;
    int pos;
    pos = 1;
    token = strtok(text, " \t\n");
    while(token != NULL){
        lowering(token, lowered);
        addword(lowered, pos++);
        token = strtok(NULL, " \t\n");
    }
}

// Show dictionary
void showdictionary(){
    int i, j;
    i = j = 0;
    while(i < wordcount){
        printf("%s | count = %d | length = %d | positions = ", dict[i].word, dict[i].count, dict[i].length);
        j = 0;
        while(j < dict[i].poscount){
            printf("%d ", dict[i].positions[j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

// Exact word search
void searchexact(char *searched){
    int i, j;
    i = j = 0;
    while(i < wordcount){
        if(strcmp(dict[i].word, searched) == 0){
            printf("word = %s | count = %d | length = %d | positions = ", dict[i].word, dict[i].count, dict[i].length);
            j = 0;
            while(j < dict[i].poscount){
                printf("%d ", dict[i].positions[j]);
                j++;
            }
            printf("\n");
            return;
        }
        i++;
    }
    printf("Word was not found!\n");
}

// Partial word search
void partialsearch(char *partialS){
    int i;
    int found;
    i = found = 0;
    while(i < wordcount){
        if(strstr(dict[i].word, partialS) != NULL){
            printf("%s | count = %d | length = %d\n", dict[i].word, dict[i].count, dict[i].length);
            found = 1;
        }
        i++;
    }
    if(!found) printf("No word found containing '%s'\n", partialS);
}

// Sorting helpers
int cmpAlpha(const void *a, const void *b){ return strcmp(((word*)a)->word, ((word*)b)->word); }
int cmpFreq(const void *a, const void *b){ return ((word*)b)->count - ((word*)a)->count; }
int cmpLength(const void *a, const void *b){ return ((word*)a)->length - ((word*)b)->length; }

// Global statistics
void globalStats(){
    int i, total, totalwords, maxLen, minLen, maxFreq;
    char *longest, *shortest, *mostFreq;
    i = total = totalwords = maxLen = maxFreq = 0;
    minLen = MAX_LEN;
    longest = shortest = mostFreq = NULL;

    if(wordcount == 0){
        printf("No words analyzed.\n");
        return;
    }

    while(i < wordcount){
        total += dict[i].length * dict[i].count;
        totalwords += dict[i].count;

        if(dict[i].length > maxLen){ maxLen = dict[i].length; longest = dict[i].word; }
        if(dict[i].length < minLen){ minLen = dict[i].length; shortest = dict[i].word; }
        if(dict[i].count > maxFreq){ maxFreq = dict[i].count; mostFreq = dict[i].word; }

        i++;
    }

    printf("Total words: %d\nUnique words: %d\nAverage length: %.2f\nShortest word: %s\nLongest word: %s\nMost frequent: %s (%d)\n",
           totalwords, wordcount, (double)total/totalwords, shortest, longest, mostFreq, maxFreq);
}

// Palindromes
void showpalindromes(){
    int i, j, len, flag;
    char temp;
    i = 0;
    while(i < wordcount){
        len = dict[i].length;
        flag = 1;
        j = 0;
        while(j < len/2){
            temp = dict[i].word[len-1-j];
            if(dict[i].word[j] != temp){
                flag = 0;
                break;
            }
            j++;
        }
        if(flag) printf("%s\n", dict[i].word);
        i++;
    }
}

// Compare characters for qsort
int compareChars(const void *a, const void *b){ return (*(char*)a - *(char*)b); }

// Anagrams
void showAnagrams(){
    int i, j;
    char a[MAX_LEN], b[MAX_LEN];
    i = 0;
    while(i < wordcount){
        j = i+1;
        while(j < wordcount){
            if(dict[i].length != dict[j].length){ j++; continue; }
            strcpy(a, dict[i].word);
            strcpy(b, dict[j].word);
            qsort(a, strlen(a), sizeof(char), compareChars);
            qsort(b, strlen(b), sizeof(char), compareChars);
            if(strcmp(a,b) == 0) printf("%s <=> %s\n", dict[i].word, dict[j].word);
            j++;
        }
        i++;
    }
}

// Word cloud
void wordCloud(){
    int i, j;
    i = 0;
    while(i < wordcount){
        printf("%s: ", dict[i].word);
        j = 0;
        while(j < dict[i].count){
            printf("*");
            j++;
        }
        printf("\n");
        i++;
    }
}

// Main function
int main(){
    char text[MAX_TEXT], input[MAX_LEN];
    int choice, s;

    while(1){
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

        switch(choice){
            case 1:
                printf("Enter text (max 2000 chars):\n");
                fgets(text, MAX_TEXT, stdin);
                wordcount = 0;
                analyzethetext(text);
                printf("Text analyzed successfully!\n");
                break;
            case 2: showdictionary(); break;
            case 3:
                printf("Enter word: ");
                scanf("%s", input);
                lowering(input, input);
                searchexact(input);
                break;
            case 4:
                printf("Enter substring: ");
                scanf("%s", input);
                lowering(input, input);
                partialsearch(input);
                break;
            case 5:
                printf("Sort by: 1-Alphabet 2-Frequency 3-Length: ");
                scanf("%d", &s);
                if(s==1) qsort(dict, wordcount, sizeof(word), cmpAlpha);
                else if(s==2) qsort(dict, wordcount, sizeof(word), cmpFreq);
                else if(s==3) qsort(dict, wordcount, sizeof(word), cmpLength);
                printf("Sorted!\n");
                break;
            case 6: globalStats(); break;
            case 7:
                printf("Palindromes:\n"); showpalindromes();
                printf("Anagrams:\n"); showAnagrams();
                printf("Word Cloud:\n"); wordCloud();
                break;
            case 8: exit(0);
        }
    }
}

