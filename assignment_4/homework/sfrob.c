#include <stdio.h>
#include <stdlib.h>

const int LESS = -1;
const int SAME = 0;
const int GREATER = 1;


char unfrobChar(const char c);
int frobcmp(char const * a, char const * b);
// void tester();
void run();
void run2();
void checkForInputError();
short checkForEndOfFile(char c);

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

/*
void tester(){
    char c;

    char input1[] = "*{_CIA\030\031 ";
    char input2[] = "*`_GZY\v ";

    const char * ptr1 = input1;
    const char * ptr2 = input2;

    int a = frobcmp(ptr1, ptr2);
    printf("%d", a);
};
*/

/*
void run(){
    int lettersInWord = 0, numWords = 0;
    char * myWord;
    char ** wordList;

    wordList == NULL;
    myWord = (char *)malloc(sizeof(char));
    myWord[lettersInWord] = getc(stdin); // save first character
    if(ferror(stdin)){
        fprintf(stderr, "ERROR with input, exiting program");
        exit(1);
    }
};
*/

void run2(){
    char myChar;
    char * myWord = (char *)malloc(sizeof(char *));
    char ** wordList = (char **)malloc(sizeof(char **));
    short endOfWord = 0; // 0 if it is NOT the end of the word, 1 if it IS the end of the word
    int numChars = 0;  // number of characters in the current word
    int numWords = 0; // number of words in the word list

    // checks to see if memory couldn't be allocated for myWord or wordList
    if(myWord == NULL || wordList == NULL){
        fprintf(stderr, "ERROR with memory allocation, exiting program");
        exit(1);
    }

    // wordList[0] = myWord;

    while(1){
        printf("1");
        myChar = getc(stdin);

        checkForInputError();
        if(checkForEndOfFile(myChar)){
            break;
        }

        // if we are still adding characters to the current word
        if(!endOfWord){
            myWord = (char *)realloc(myWord, (numChars + 1) * sizeof(char));
            if(myWord == NULL){
                fprintf(stderr, "ERROR with memory allocation, exiting program");
                exit(1);
            }

            if(myChar != ' '){
                myWord[numChars++] = myChar;
                wordList[numWords] = myWord;
            }

            else{
                endOfWord = 1;
            }
        }

        else{
            printf("2");
            if(myChar == ' '){
                continue;
            }

            wordList[numWords++] = myWord;

            myWord = (char *)malloc(sizeof(char *));
            if(myWord == NULL) {
                fprintf(stderr, "ERROR with memory allocation, exiting program");
                exit(1);
            }

            wordList = (char **)realloc(wordList, (numWords + 1) * sizeof(char));
            if(wordList == NULL){
                fprintf(stderr, "ERROR with memory allocation, exiting program");
                exit(1);
            }

            numChars = 0;
            myWord[numChars++] = myChar;
            wordList[numWords] = myWord;
            endOfWord = 0;
        }
    }
    printf("3");
    qsort(wordList, numWords + 1, sizeof(char**), frobcmp);

    for(int i = 0; i < numWords; i++){
        printf(wordList[i]);
    }
    printf("4");

}


void checkForInputError(){
    if(ferror(stdin)){
        fprintf(stderr, "ERROR with input, exiting program");
        exit(1);
    }
}

short checkForEndOfFile(char c){
    if(c == EOF){
        printf("File reading is completed\n");
        return 1;
    }
    return 0;
}

int main(void){
    run2();
};