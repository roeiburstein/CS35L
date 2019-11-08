#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>


const int LESS = -1;
const int SAME = 0;
const int GREATER = 1;
char ignore_case = 0;

char unfrobChar(const char c);
int frobcmp(char const * a, char const * b);
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
    if(ignore_case) {
        return toupper(c ^ 42);
    }
    return (c ^ 42);
};

short checkForEndOfFile(char c){
    if(c == EOF){
        return 1;
    }
    return 0;
}

int main(int argc, char** argv){
    fprintf(stdout, "hi!");
    if (argc > 2){
        fprintf(stderr, "ERROR too many arguments");
        exit(1);
    }

    if (argc == 2 && strcmp(argv[1], "-f") == 0){
        ignore_case = 1;
    }

    else if (argc == 2 && strcmp(argv[1], "-f") != 0) {
        fprintf(stderr, "ERROR only -f argument allowed");
        exit(1);
    }

    char myChar;
    char * myWord = (char *)malloc(sizeof(char));
    char ** wordList = (char **)malloc(sizeof(char *));

    short endOfWord = 0; // 0 if it is NOT the end of the word, 1 if it IS the end of the word
    int numChars = 0;  // number of characters in the current word
    int numWords = 0; // number of words in the word list
    int prevNumChars = -1; // number of characters of previous word
    ssize_t result;

    // checks to see if memory couldn't be allocated for myWord or wordList
    if(myWord == NULL || wordList == NULL){
        fprintf(stderr, "ERROR with memory allocation, exiting program");
        exit(1);
    }

    struct stat file;
    if (fstat(0, &file) < 0){
        fprintf(stderr, "ERROR no file found");
        exit(1);
    }

    char * buffer;
    int buffCount = 0, maxBuffSize = 0;

    if(S_ISREG(file.st_mode)){
        buffer = (char *)malloc(file.st_size * sizeof(char));
        if(buffer == NULL){
            fprintf(stderr, "ERROR with memory allocation, exiting program");
            exit(1);
        }

        result = read(0, buffer, file.st_size);
        if(result < 0){
            fprintf(stderr, "ERROR problem reading input, exiting program");
            exit(1);
        }
        maxBuffSize = file.st_size;
    }

    if(buffCount > maxBuffSize - 1){
        result = read(0, &myWord, sizeof(char));
        if(result < 0){
            fprintf(stderr, "ERROR problem reading input, exiting program");
            exit(1);
        }
    }

    else{
        myWord[numChars++] = buffer[buffCount++];
        result = 1;
    }
    while(1){

        if(buffCount > maxBuffSize - 1){
            result = read(0, &myChar, sizeof(char));
            if(result < 0){
                fprintf(stderr, "ERROR problem reading input, exiting program");
                exit(1);
            }

            else if(result == 0){
                wordList[numWords++] = myWord;
                if(wordList[numWords - 1][prevNumChars - 1] != ' ') {

                    myWord = (char*) realloc(myWord, (numChars + 1) * sizeof(char));
                    myWord[numChars] = ' ';
                    wordList[numWords - 1] = myWord;

                }
                break;
            }
        }

        else{
            myChar = buffer[buffCount++];
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

        // if we have reached the end of our current word
        else{
            while (myChar == ' ') {
                if(buffCount >= maxBuffSize){
                    result = read(0, &myChar, sizeof(char));
                    if(result < 0){
                        fprintf(stderr, "ERROR problem reading input, exiting program");
                        exit(1);
                    }
                }
                else{
                    myChar = buffer[buffCount++];
                }
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
            if (write(1, &wordList[x][y], sizeof(char)) < 0){
                fprintf(stderr, "ERROR with printing character, exiting program");
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

