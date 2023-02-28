#include "p3.h"

int main(int args, char* argv[]) {

    system("clear");

    char userFile[STDSTRING];  //unmodified storage of userFile

    if (args > 2) {
        printf("Too many arguments given from command line.\n");
        printf("Argument should be limited only to a valid userFile.\n");
        exit(1);
    }

    else if (args == 2) {
        strcpy(userFile, argv[1]);
        strcat(userFile, fileextension);  // apply extension    
        FILE* infile = fopen(userFile, readmode);
        if (infile == NULL) {
            printf("Input File Error: %s\n", userFile);
            exit(1);
        }
        printf("File: %s", userFile);
        filter(infile, tempfileName);  // Check for comments and put scrubbed code into temporary file

        fclose(infile);
    }

    else if (args == 1) {        //keyboard processing             
        char keybdIn[WHOLELINE];

        FILE* kbdFileOut = fopen(keyboardfileName, readwritemode);  // Create file to store keyboard input
        if (kbdFileOut == NULL) {
            printf("Input File Error: %s\n", keyboardfileName);
            exit(1);
        }

        printf("Enter data now. Press Ctrl D when finished.\n");
        while (fgets(keybdIn, sizeof(keybdIn), stdin)) { // get input from stdin, store in keybdIn
            fputs(keybdIn, kbdFileOut);
        }

        printf("\n\n");
        rewind(kbdFileOut);  // resets file pointer to the beginning

        filter(kbdFileOut, tempfileName); // Check for comments and put scrubbed code into temporary file

        fclose(kbdFileOut);
        int deleteFlag = remove(keyboardfileName); // Delete keyboard file
        if (deleteFlag != 0)
            printf("*** WARNING ***\nTemporary file: %s was not removed.\n", keyboardfileName);
    }


    FILE* infileMain = fopen(tempfileName, readmode);
    if (infileMain == NULL) {
        printf("Temporary output File Error: %s\n", tempfileName);
        printf("You should NOT be seeing this error at this point.\n");
        exit(1);
    }
    if (NULL != infileMain) {  // determine if file is empty
        fseek(infileMain, 0, SEEK_END);
        int size = ftell(infileMain);
        if (size == 0) {
            printf("Error: file is empty\n");
            exit(1);
        }
    }

    rewind(infileMain);  // resets file pointer to the beginning

    buildFSAtable();

    tokens = malloc(1000 * sizeof(Token));  // create Tokens

    char intext[WHOLELINE];
    int lineNum = 1;
    int tokenIndex = 0;

    while (fgets(intext, sizeof(intext), infileMain)) {
        tokenIndex = driver(intext, lineNum, tokenIndex); // process temp file with driver          
        lineNum++;
    }

    strcpy(tokens[tokenIndex].text, "EOF\0");  // append tokens with EOT for the end of file
    tokens[tokenIndex].tkType = END_OF_FILE;
    tokens[tokenIndex].lineNumber = (lineNum);

    keywords(tokenIndex + 1); // process keywords   
    int deleteFlag = remove(tempfileName);  // delete tempfile
    if (deleteFlag != 0)
        printf("*** WARNING ***\nTemporary file: %s was not removed.\n", tempfileName);

    tree = (treeNode*)malloc(1000* sizeof(treeNode));
    tree = parser();   

    semanticCheck(tree);

    free(tree);

    printf("  ### PROCESS COMPLETED SUCCESSFULLY ###\n\n");  // Tidy up terminal output
    return 0;
}


