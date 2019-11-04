#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    if (argc == 3) {
        const char * from = argv[1], * to = argv[2];
        size_t fromLen = strlen(from);
        size_t toLen = strlen(to);
        short asciiArray[255] = {0};

        if(fromLen != toLen) {
            fprintf(stderr, "ERROR, first and second arguments are different lengths, exiting program");
            exit(1);
        }

        for(size_t i = 0; i < fromLen; i++) {
            if(asciiArray[from[i]] == 1) {
                fprintf(stderr, "ERROR, repeating characters in first argument, exiting program");
                exit(1);
            }
            asciiArray[(int)from[i]] = 1;
        }

        char c;

        if(read(0, &c, sizeof(char)) < 0) {
            fprintf(stderr, "ERROR, problem reading input, exiting program");
            exit(1);
        }
        ssize_t result = -1;
        while(result != 0) {
            result = read(0, &c, sizeof(char));
            if(asciiArray[c] == 1) {
                size_t charIndex = -1;
                for(size_t j = 0; j < fromLen; j++) {
                    if(from[j] ==  c) {
                        charIndex = j;
                    }
                }
                write(1, &to[charIndex], sizeof(char));
            }
            else {
                write(1, &c, sizeof(char));
            }
        }
    }





    //abc xyz
    //abccba
}