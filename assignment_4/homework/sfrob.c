#include <stdio.h>
#include <stdlib.h>

const int LESS = -1;
const int SAME = 0;
const int GREATER = 1;

/*
 * returns an int result that is negative, zero, or positive depending on
 * whether a is less than, equal to, or greater than b.
 */
char unfrobChar(const char c);
int frobcmp(char const * a, char const * b);
void tester();

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
 * returns result of the character exclusive OR 42
 */
char unfrobChar(const char c){
    return (c ^ 42);
};

void tester(){
    char c;

    char input1[] = "*{_CIA\030\031 ";
    char input2[] = "*`_GZY\v ";

    const char * ptr1 = input1;
    const char * ptr2 = input2;

    int a = frobcmp(ptr1, ptr2);
    printf("%d", a);
};

int main(void){
    int lettersInWord = 0, numWords = 0;
    char * myWord;
    char ** wordList;

    wordList == NULL;
    myWord = (char *)malloc(sizeof(char));
    myWord[lettersInWord] = getc(stdin); // save first character
    if(ferror(stdin)){
        stderr
    }

};