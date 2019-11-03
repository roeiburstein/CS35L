#include <stdio.h>
#include <stdlib.h>

const int LESS = -1;
const int SAME = 0;
const int GREATER = 1;


char unfrobChar(const char c);
int frobcmp(char const * a, char const * b);
void run2();
void checkForInputError();
short checkForEndOfFile(char c);
int compare(const void * a, const void * b);

/*
 * function that runs frobcmp as input for qsort
 */
int compare(const void * a, const void * b){
    return frobcmp(*(char**) a, *(char**) b);
}

/*
 * compares two frobnicated characters a and b
 * returns int result -1 if a < b, 0 if a = b, and 1 if a > b
 */
int frobcmp(char const * a, char const * b) {
    for(; ; a++, b++){
        if(*a == ' ' && *b == ' '){
            return SAME;
        }

        while(*a == '\0' || *b == '\0'){ // increments a and/or b array if current value is NULL
            if(*a == '\0'){
                ++a;
            }

            if(*b == '\0') {
                ++b;
            }
        }

        if(*a == ' ' && *b != ' ') {
            return LESS;
        }

        else if(*b == ' ' && *a != ' ') {
            return GREATER;
        }

        if(*a != *b) return (unfrobChar(*a) > unfrobChar(*b)) ? GREATER : LESS;
    }
};

/*
 * unfrobnicates character by undoing memfrob()
 * returns result of the character exclusive OR 42 (101010 in binary)
 */
char unfrobChar(const char c){
    return (c ^ 42);
};

void run2(){
    // printf("I ran run2\n");
    char myChar;
    char * myWord = (char *)malloc(sizeof(char));
    char ** wordList = (char **)malloc(sizeof(char *));
    short endOfWord = 0; // 0 if it is NOT the end of the word, 1 if it IS the end of the word
    int numChars = 0;  // number of characters in the current word
    int numWords = 0; // number of words in the word list
    int prevNumChars = -1; // number of characters of previous word

    // checks to see if memory couldn't be allocated for myWord or wordList
    if(myWord == NULL || wordList == NULL){
        fprintf(stderr, "ERROR with memory allocation, exiting program");
        exit(1);
    }
    wordList[0] = myWord;

    while(1){
        myChar = getc(stdin);
        checkForInputError();
        if(checkForEndOfFile(myChar)){
            if(wordList[numWords - 1][prevNumChars - 1] != ' ') {

                myWord = (char*) realloc(myWord, (numChars + 1) * sizeof(char));
                myWord[numChars] = ' ';
                wordList[numWords - 1] = myWord;

            }
            break;
        }

        // if we are still adding characters to the current word
        if(!endOfWord){
            myWord = (char *)realloc(myWord, (numChars + 1) * sizeof(char));
            if(myWord == NULL){
                fprintf(stderr, "ERROR with memory allocation, exiting program");
                exit(1);
            }
            myWord[numChars++] = myChar;

            if(myChar == ' '){
                endOfWord = 1;
            }
        }

        else{
            if(myChar == ' ') {
                continue;
            }
            wordList[numWords++] = myWord;

            myWord = (char *)malloc(sizeof(char *));
            if(myWord == NULL) {
                fprintf(stderr, "ERROR with memory allocation, exiting program");
                exit(1);
            }

            wordList = (char **)realloc(wordList, (numWords + 1) * sizeof(char *));
            if(wordList == NULL){
                fprintf(stderr, "ERROR with memory allocation, exiting program");
                exit(1);
            }

            prevNumChars = numChars;
            numChars = 0;
            myWord[numChars++] = myChar;
            endOfWord = 0;
        }
    }

    qsort(wordList, numWords, sizeof(char**), compare);
    for (int x = 0; x < numWords; x++){
        int y = 0;
        while(1){
            if (putchar(wordList[x][y]) == EOF){
                fprintf(stderr, "Error printing characters.");
                exit(1);
            }

            if (wordList[x][y] == ' '){
                break;
            }
            y++;
        }
        free(wordList[x]);
    }
    free(wordList);
    exit(0);
}


void checkForInputError(){
    if(ferror(stdin)){
        fprintf(stderr, "ERROR with input, exiting program");
        exit(1);
    }
}

short checkForEndOfFile(char c){
    if(c == EOF){
        return 1;
    }
    return 0;
}

int main(void){
    run2();
};