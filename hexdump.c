#include <stdio.h>
#include <stdbool.h> 
#include <string.h>

#define LINE_LEN 16

void printByteOffset(int byteNum, bool asciiMode) {
    if (asciiMode){
        if (byteNum == 0){ 
            printf("%08x", byteNum);
        }

        if (byteNum >= LINE_LEN && byteNum % LINE_LEN == 0) {
            printf("\n%08x", byteNum);
        }
    } else {
        if (byteNum == 0){ 
            printf("%07x", byteNum);
        }

        if (byteNum >= LINE_LEN && byteNum % LINE_LEN == 0) {
            printf("\n%07x", byteNum);
        }
    }
}

int main( int argc, char *argv[] ) {
    if( argc > 1 && argc <= 3 ) {
        
        bool asciiMode = false;
        bool byteMode = false;
        bool normalMode = argc == 2;

        if (argc == 3) {
            asciiMode = (strcmp(argv[1], "-C") == 0); 
            byteMode = (strcmp(argv[1], "-b") == 0);

            if (!asciiMode && !byteMode){
                printf("Invalid argument format. Please use argument format: ./a.out [-b | -C] file \n");
                return -1;
            }
        }

        FILE* fp;
        
        if (asciiMode || byteMode){
            fp = fopen(argv[2], "rb");
        } else {
            fp = fopen(argv[1], "rb");
        }

        if (fp == NULL) {
            printf("file not found\n");
            return -1;
        }

        int byteNum = 0;  // keep track of how many bytes are read
        int b;  // buffer for each byte read
        int index = 0;  // used in ascii mode
        int charsPrinted = 0; // used in ascii mode
        char bytesRead[LINE_LEN]; // used in ascii mode
        while((b = getc(fp)) != EOF){

            printByteOffset(byteNum, asciiMode);

            // no flags
            if (normalMode){
                int a = getc(fp);
                byteNum++;

                if (a != EOF){
                    printf(" %02x%02x", a, b);
                } else {
                    printf(" 00%02x", b);
                    byteNum--;
                } 
            }

            // -b flag
            if (byteMode){
                printf(" %03o", b);
            }

            // -C flag
            if (asciiMode){
                if (byteNum % (LINE_LEN /2 ) == 0){
                    printf("  %02x", b);
                    index += 4;
                } else {
                    printf(" %02x", b);
                    index += 3;
                }

                charsPrinted++;
                bytesRead[byteNum % LINE_LEN] = b;

                // print the ascii output
                if ((byteNum > 0 && (byteNum + 1) % (LINE_LEN) == 0)) {
                    printf("  |");
                    for (int i = 0; i < LINE_LEN; i++){
                        if (isprint(bytesRead[i])){
                            printf("%c", bytesRead[i]);
                        } else {
                            printf(".");
                        }
                    }
                    printf("|");

                    //clear bytes read
                    for (int i = 0; i < LINE_LEN; i++){
                        bytesRead[i] = ' ';
                    }
                    index = 0;
                    charsPrinted = 0;
                }
            }

            byteNum++;
        }

        // print the final line of the output
        if (byteNum > 0){
            if (asciiMode){
                if (index > 0){
                    // magic val 50 for the length of the lines
                    for (int i = index; i < 50 ; i ++){
                        printf(" "); // print enough space for the |hello world ff| at the end
                    }
                    printf("  |");
                    for (int i = 0; i < charsPrinted; i++){
                        if (isprint(bytesRead[i])){
                            printf("%c", bytesRead[i]);
                        } else {
                            printf(".");
                        }
                    }
                    printf("|"); 
                }
                printf("\n%08x\n", byteNum);
            } else {
                printf("\n%07x\n", byteNum);
            }
        }

        fclose(fp);
        // printf("\n%d\n", charsPrinted);
    }
    return 0;
}