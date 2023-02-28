#ifndef STATICSEMANTIC_H
#define STATICSEMANTIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include"node.h"
#include"token.h"


void store(Token);
void stack_search(Token);
bool declared(char*);
void semanticCheck(treeNode*);

#define MAX 1000


typedef struct {

	int scopeDepth;
	int lineNumber;
	char text[9];  // String

} Scope;




#endif // !STATICSEMANTIC_H

