#ifndef P2_H
#define P2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "filter.h"
#include "fsaTable.h"
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "node.h"
#include "staticSemantic.h"


#define WHOLELINE 500 // anticipating a line not being larger than 
#define STDSTRING 40  // Sets 40 chars as the standard-use array

static const char tempfileName[] = "temp";
static const char keyboardfileName[] = "keyboardtemp";
static const char fileextension[] = ".sp2020";

const char* readmode = "r";  // these are used to bypass an error that showed up out of nowhere
const char* readwritemode = "w+";


#endif // !P2_H
