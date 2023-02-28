#include "staticSemantic.h"


static int scopeVars[MAX];	// This keeps track how many variables are being store. 2D for multi-scope implications.
static int scope = 0;			// Index for scope
Scope stack[MAX][MAX];		// variable storage
int i = 0;				// all counters


/*
Stores variable name, line number, and scope depth in stack.
Also prevents stack overflow.
*/
void store(Token stackToken) {
	if (scope == (MAX-10)) 
		printf("		**SEMANTIC Warning: Stack is approaching limit. **\n");		

	else if (scope == MAX) {
		printf("  ##### SEMANTIC FAILURE 01: Stack is full.#####\n");
		exit(1);
	}

	size_t len = sizeof(stackToken.text) / sizeof(stackToken.text[0]);
	strncpy(stack[scope][scopeVars[scope]].text, stackToken.text, len);  // store variable info

	stack[scope][scopeVars[scope]].scopeDepth = scope;  // store scope location for for multi-scope implementation
	stack[scope][scopeVars[scope]].lineNumber = stackToken.lineNumber;
}


/*
Searches the stack for declared variables.
Exits if found.
*/
void stack_search(Token in) {
	for (i = scopeVars[scope]; i >= 0; i--) {
		//printf("i: %d.   scopeVars[scope] = %d.   stackIndex: %d   scope:%d   intext='%s'  ||'%s'=stack text\n", i, scopeVars[scope], stackIndex, scope, inText, stack[scope][i].text);
		if (strcmp(stack[scope][i].text, in.text) == 0) {
			printf("  ### SEMANTIC ERROR 02 [Line:%d]: '%s' - {Variable previously declared} Line:%d\n\n",in.lineNumber, in.text, stack[scope][i].lineNumber);
			exit(1);
		}
	}
}


/*
Searches the stack to ensure variable has been previously declared.
*/
bool declared(char* var) {
	/*
	for (i = scopeVars[scope]; i >= 0; i--) { // This is for for multi-scope implementation 
		if (strcmp(stack[0][i].text, var) == 0) {
			return 1;
		}
	}  */	

	for (i = scopeVars[scope]; i >= 0; i--) { //local check
		if (strcmp(stack[scope][i].text, var) == 0) {
			return true;
		}
	}
	
	return false;
}


/*
Runs through a tree looking for variables based on specific BNF
called my p3.c
*/
void semanticCheck(treeNode* current) {
	if (current == NULL)
		return;

	if (strcmp(current->ident, "<program>") == 0) {
		if (current->c1 != NULL)
			semanticCheck(current->c1);

		if (current->c2 != NULL)
			semanticCheck(current->c2);
	}
	else if (strcmp(current->ident, "<block>") == 0) {
		/*if (current->t1.tkType == BEGIN_KYD);  scope++; */	// for multi-scope implementation
			
		if (current->c1 != NULL)
			semanticCheck(current->c1);
		if (current->c2 != NULL)
			semanticCheck(current->c2);
		
		/*if (current->t1.tkType == END_KYD);  scope--;*/   // for multi-scope implementation
	}
	else if (strcmp(current->ident, "<vars>") == 0) {  // Variable stored in token 2. Check for redeclare
		stack_search(current->t2);  // search for previously defined variables
		scopeVars[scope]++;
		store(current->t2);

		if (current->c1 != NULL)
			semanticCheck(current->c1);
	}

	/* #######  Statements below check for declaration																				#######*/

	else if (strcmp(current->ident, "<in>") == 0) {  //<in> [variable stored in token 2]
		if (!declared(current->t2.text)) {
			printf("  ### SEMANTIC ERROR 03 [Line:%d]: '%s' - {Attempted use of undeclared variable.}\n\n", current->t2.lineNumber, current->t2.text);
			exit(1);
		}
		if (current->c1 != NULL)
			semanticCheck(current->c1);
	}
	else if ((strcmp(current->ident, "<R>") == 0)) { //<r> [variable stored in token 1] but not always. Check token type
		if (current->t1.tkType == IDENTIFIER) {
			if (!declared(current->t1.text)) {
				printf("  ### SEMANTIC ERROR 03 [Line:%d]: '%s' - {Attempted use of undeclared variable.}\n\n", current->t1.lineNumber, current->t1.text);
				exit(1);
			}
		}
		if (current->c1 != NULL)
			semanticCheck(current->c1);
	}
	else if ((strcmp(current->ident, "<assign>") == 0)) { //<assign> [variable stored in token 1]
		if (current->t1.tkType == IDENTIFIER) {
			if (!declared(current->t1.text)) {
				printf("  ### SEMANTIC ERROR 03 [Line:%d]: '%s' - {Attempted use of undeclared variable.}\n\n", current->t1.lineNumber, current->t1.text);
				exit(1);
			}
		}
		if (current->c1 != NULL)
			semanticCheck(current->c1);
	}

	/* #######  Everything else																		#######*/

	else {									
		if (current->c1 != NULL)
			semanticCheck(current->c1);

		if (current->c2 != NULL)
			semanticCheck(current->c2);

		if (current->c3 != NULL)
			semanticCheck(current->c3);

		if (current->c4 != NULL)
			semanticCheck(current->c4);
	}

	return;
}
