#include "scanner.h"

//keyword list used for keywords() function
char* keywordlist[] = { "begin", "end", "loop", "void", "var",
                    "return", "in", "out", "program", "iffy",
                    "then", "let", "data" };




/* driver()
Reads each char fed through corrent to determine tokentype from FSA Table.
Stores in struct char by char in addition to token type and line number.
Used in: p2.c */
int driver(char* current, int linenumber, int tokenIndex) {


    int next = 0;
    int m = 0;
    int state = 0;
    int instanceIndex = 0;  // used for text length
    

    for (m = 0; m < strlen(current); m++) {

        char tempChar = current[m];  // used to bypass error
       

        next = character_id(tempChar, linenumber);


        if (next == -1) {
            printf(" Error: Invalid character found on line: %d position: %d: '%c'\n", linenumber, m, tempChar);
            
            exit(1);
        }

        else if ((next == 3) && (current[m + 1] == '=') && (current[m - 1] != '=')) { 
            next = 6;
        }

        state = FSAtable[state][next];


        if (state == -1) {  // invalid character error
            printf("function: tokenID() error unknown. state: %d, line number: %d\n", state, linenumber);
            
            exit(1);
        }

        if (state == 1001)
            tokens[tokenIndex].tkType = IDENTIFIER;

        else if (state == 1002)
            tokens[tokenIndex].tkType = INTEGER;

        else if (state == 1003)
            tokens[tokenIndex].tkType = EQUAL;

        else if (state == 1004)
            tokens[tokenIndex].tkType = LEFT_ANGLE_BRACKET;

        else if (state == 1005)
            tokens[tokenIndex].tkType = RIGHT_ANGLE_BRACKET;

        else if (state == 1006) {
            tokens[tokenIndex].tkType = DOUBLE_EQUAL;
            tokens[tokenIndex].lineNumber = linenumber;
            tokens[tokenIndex].text[instanceIndex] = tempChar;
            state = 0;
            tokenIndex++;
            instanceIndex = 1;
        }
        else if (state == 1007)
            tokens[tokenIndex].tkType = COLON;

        else if (state == 1008)
            tokens[tokenIndex].tkType = PLUS;

        else if (state == 1009)
            tokens[tokenIndex].tkType = MINUS;

        else if (state == 1010)
            tokens[tokenIndex].tkType = ASTERISK;

        else if (state == 1011)
            tokens[tokenIndex].tkType = SLASH;

        else if (state == 1012)
            tokens[tokenIndex].tkType = PERCENT;

        else if (state == 1013)
            tokens[tokenIndex].tkType = PERIOD;

        else if (state == 1014)
            tokens[tokenIndex].tkType = LEFT_PARENTHESIS;

        else if (state == 1015)
            tokens[tokenIndex].tkType = RIGHT_PARENTHESIS;

        else if (state == 1016)
            tokens[tokenIndex].tkType = COMMA;

        else if (state == 1017)
            tokens[tokenIndex].tkType = LEFT_CURLY_BRACKET;

        else if (state == 1018)
            tokens[tokenIndex].tkType = RIGHT_CURLY_BRACKET;

        else if (state == 1019)
            tokens[tokenIndex].tkType = SEMICOLON;

        else if (state == 1020)
            tokens[tokenIndex].tkType = LEFT_SQUARE_BRACKET;

        else if (state == 1021)
            tokens[tokenIndex].tkType = RIGHT_SQUARE_BRACKET;


        if (state >= 1001 && state <= 1021) { // happens when WS occurs
            tokens[tokenIndex].lineNumber = linenumber;
            tokens[tokenIndex].text[instanceIndex] = '\0';
            instanceIndex = 0;
            state = 0;
            tokenIndex++;  // start next word
            m--;
        }

        if (!isspace(tempChar)) {  
            tokens[tokenIndex].text[instanceIndex] = tempChar;
            instanceIndex++;
        }

        

    } // end of for loop m

    return tokenIndex;
}


/* character_id()
identifies characters passed to fuction.
characters selected from project description.
Used in: driver() */
int character_id(char input, int linenumber) {

    if (isalpha(input))  // alpha check
        return 0;

    else if (input == '\n' || input == ' ' || input == '\t')  // white space check
        return 1;

    else if (isdigit(input))
        return 2;

    else if (input == '=')
        return 3;

    else if (input == '<')
        return 4;

    else if (input == '>')
        return 5;

    else if (input == ':')
        return 7;

    else if (input == '+')
        return 8;

    else if (input == '-')
        return 9;

    else if (input == '*')
        return 10;

    else if (input == '/')
        return 11;

    else if (input == '%')
        return 12;

    else if (input == '.')
        return 13;

    else if (input == '(')
        return 14;

    else if (input == ')')
        return 15;

    else if (input == ',')
        return 16;

    else if (input == '{')
        return 17;

    else if (input == '}')
        return 18;

    else if (input == ';')
        return 19;

    else if (input == '[')
        return 20;

    else if (input == ']')
        return 21;

    return -1;  // unrecognized character

}


/* keywords()
Clarifies keywords from identifier tokens
keywords selected from project description
Used in: p2.c */
void keywords(int tokenIndex) {

    int i;
    int k;
    size_t len = sizeof(keywordlist) / sizeof(keywordlist[0]);  // taken from StackOVerflow example

    for (i = 0; i < tokenIndex; i++) {

        if (tokens[i].tkType == IDENTIFIER) {

            for (k = 0; k < len; k++) {
                if (strcmp(tokens[i].text, "begin\0") == 0) {
                    tokens[i].tkType = BEGIN_KYD;
                }

                if (strcmp(tokens[i].text, "end\0") == 0) {
                    tokens[i].tkType = END_KYD;
                }

                if (strcmp(tokens[i].text, "loop\0") == 0) {
                    tokens[i].tkType = LOOP_KYD;
                }

                if (strcmp(tokens[i].text, "void\0") == 0) {
                    tokens[i].tkType = VOID_KYD;
                }

                if (strcmp(tokens[i].text, "var\0") == 0) {
                    tokens[i].tkType = VAR_KYD;
                }

                if (strcmp(tokens[i].text, "return\0") == 0) {
                    tokens[i].tkType = RETURN_KYD;
                }

                if (strcmp(tokens[i].text, "in\0") == 0) {
                    tokens[i].tkType = IN_KYD;
                }

                if (strcmp(tokens[i].text, "out\0") == 0) {
                    tokens[i].tkType = OUT_KYD;
                }

                if (strcmp(tokens[i].text, "program\0") == 0) {
                    tokens[i].tkType = PROGRAM_KYD;
                }

                if (strcmp(tokens[i].text, "iffy\0") == 0) {
                    tokens[i].tkType = IFFY_KYD;
                }

                if (strcmp(tokens[i].text, "then\0") == 0) {
                    tokens[i].tkType = THEN_KYD;
                }

                if (strcmp(tokens[i].text, "let\0") == 0) {
                    tokens[i].tkType = LET_KYD;
                }

                if (strcmp(tokens[i].text, "data\0") == 0) {
                    tokens[i].tkType = DATA_KYD;
                }
            }
        }
    }
}





