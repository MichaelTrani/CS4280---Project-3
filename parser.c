#include "parser.h"


Token tk;

int tk_index; //to move through ta array
int ff = 0; // keep track of missing error messages

static const char* tokenName[] = {  // token to text for error messages
	[BEGIN_KYD] = "begin_tk",[END_KYD] = "end_tk",[LOOP_KYD] = "loop_tk",[VOID_KYD] = "void_tk",[VAR_KYD] = "var_tk",[RETURN_KYD] = "return_tk",[IN_KYD] = "in_tk",[OUT_KYD] = "out_tk",
	[PROGRAM_KYD] = "program_tk",[IFFY_KYD] = "iffy_tk",[THEN_KYD] = "then_tk",[LET_KYD] = "let_tk",[DATA_KYD] = "data_tk",[END_OF_FILE] = "EOF_tk",[IDENTIFIER] = "identifier_tk",
	[INTEGER] = "integer_tk",[EQUAL] = "equal_tk",[LEFT_ANGLE_BRACKET] = "left_angle_bracket_tk",[RIGHT_ANGLE_BRACKET] = "right_angle_bracket_tk",[DOUBLE_EQUAL] = "double_equal_tk",
	[COLON] = "colon_tk",[PLUS] = "plus_tk",[MINUS] = "minus_tk",[ASTERISK] = "asterisk_tk",[SLASH] = "slash_tk",[PERCENT] = "percent_tk",[ERROR] = "error_tk",
	[PERIOD] = "period_tk",[LEFT_PARENTHESIS] = "left_parenthesis_tk",[RIGHT_PARENTHESIS] = "right_parenthesis_tk",[COMMA] = "comma_tk",[LEFT_CURLY_BRACKET] = "left_curly_bracket_tk",
	[RIGHT_CURLY_BRACKET] = "right_curly_bracket_tk",[SEMICOLON] = "semicolon_tk",[LEFT_SQUARE_BRACKET] = "left_square_bracket_tk",[RIGHT_SQUARE_BRACKET] = "right_square_bracket_tk"
};


treeNode* parser() {
	tk_index = 0;

	treeNode *root;
	tk = tokens[tk_index];
	root = program();

	if (tk.tkType != END_OF_FILE) {
		printf("error: eof expected but not received\nToken info:\n");
		printf("Token string: %s\nLine number: %d\nToken received: %s\n", tk.text, tk.lineNumber, tokenName[tk.tkType]);
		exit(1);
	}

	if (ff > 0)
		printf("Hey dummy, you forgot to implement: %d error messages!\n", ff);

	return root;
}


//<program>  ->     <vars> <block>
treeNode* program(){
	int depth = 0;

	treeNode *temp = nodebuild("<program>", depth);
	temp->c1 = vars(depth);

	if (temp->c1 == NULL) {
		temp->c1 = block(depth);
	}
	else {
		temp->c2 = block(depth);
	}

	return temp;
}


//<block>       ->      begin <vars> <stats> end
treeNode* block(int depth){
	depth++;
	treeNode* temp = nodebuild("<block>", depth);

	if (tk.tkType == BEGIN_KYD) {
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index]; // advances token array

		temp->c1 = vars(depth);

		if (temp->c1 == NULL) {
			temp->c1 = stats(depth);
		}
		else{
			temp->c2 = stats(depth);
		}

		if (tk.tkType == END_KYD) {
			temp->t2 = tk;
			tk_index++; tk = tokens[tk_index];
			return temp;
		}
		else {
			printf("\n  PARSER ERROR ### <BLOCK> Error - Invalid Token Line %d: \nExpected: END_KYWD\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
			exit(1);
		}
	}
	else {
		printf("\n### <BLOCK> Error - Invalid Token Line %d: \nExpected: BEGIN_KYWD\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
		exit(1);
	}
}


//<vars>         ->      empty | data Identifier =  Integer  .  <vars>
treeNode* vars(int depth){
	depth++;
	treeNode *temp = nodebuild("<vars>", depth);

	if (tk.tkType == DATA_KYD) {
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index];

		if (tk.tkType == IDENTIFIER) {
			temp->t2 = tk;
			tk_index++; tk = tokens[tk_index];

			if (tk.tkType == EQUAL) {
				temp->t3 = tk;
				tk_index++; tk = tokens[tk_index];

				if (tk.tkType == INTEGER) {
					temp->t4 = tk;
					tk_index++; tk = tokens[tk_index];	

					if (tk.tkType == PERIOD) {
						temp->t5 = tk;

						tk_index++; tk = tokens[tk_index];
						temp->c1 = vars(depth);

						return temp;
					}
					else {
						printf("\n### <VARS> Error - Invalid Token Line %d: \nExpected: PERIOD\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
						exit(1);
					}
				}
				else {
					printf("\n### <VARS> Error - Invalid Token Line %d: \nExpected: INTEGER\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
					exit(1);
				}
			}
			else {
				printf("\n### <VARS> Error - Invalid Token Line %d: \nExpected: EQUAL\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
				exit(1);
			}
		}
		else {
			printf("\n### <VARS> Error - Invalid Token Line %d: \nExpected: IDENTIFIER\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
			exit(1);
		}
	}
	else
		return NULL;
}


//<expr>        ->      <N> - <expr>  | <N>
treeNode* expr(int depth){
	depth++;
	treeNode* temp = nodebuild("<expr>", depth);
	temp->c1 = N(depth);

	if (tk.tkType == MINUS) {
		temp->t1 = tk;

		tk_index++; tk = tokens[tk_index];
		temp->c2 = expr(depth);

		return temp;
	}
	else 
		return temp;
}


//<N>             ->       <A> / <N> | <A> * <N> | <A>
treeNode* N(int depth){
	depth++;
	treeNode* temp = nodebuild("<N>", depth);

	temp->c1 = A(depth);

	if (tk.tkType == SLASH){ 
		temp->t1 = tk;

		tk_index++; tk = tokens[tk_index];
		temp->c2 = N(depth);

		return temp;
	}
	else if (tk.tkType == ASTERISK) {
		temp->t1 = tk;

		tk_index++; tk = tokens[tk_index];
		temp->c2 = N(depth);

		return temp;
	}
	else 
		return temp;
}


//<A>             ->        <M> + <A> | <M>
treeNode* A(int depth){
	depth++;

	treeNode* temp = nodebuild("<A>", depth);

	temp->c1 = M(depth);

	if (tk.tkType == PLUS) {
		temp->t1 = tk;

		tk_index++; tk = tokens[tk_index];
		temp->c2 = A(depth);

		return temp;
	}
	else 
		return temp;
}


//<M>              ->     * <M> |  <R>
treeNode* M(int depth){
	depth++;

	treeNode* temp = nodebuild("<M>", depth);

	if (tk.tkType == ASTERISK) {
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index];
		temp->c1 = M(depth);

		return temp;
	}
	else {
		temp->c1 = R(depth);
		return temp;
	}
}


//<R>              ->      ( <expr> ) | Identifier | Integer
treeNode* R(int depth){
	depth++;
	treeNode* temp = nodebuild("<R>", depth);

	if (tk.tkType == LEFT_PARENTHESIS){
		temp->t1 = tk;

		tk_index++; tk = tokens[tk_index];
		temp->c1 = expr(depth);

		if (tk.tkType == RIGHT_PARENTHESIS) {
			temp->t2 = tk;

			tk_index++; tk = tokens[tk_index];
			return temp;
		}
		else{
			printf("\n### <R> Error - Invalid Token Line %d: \nExpected: RIGHT_PARENTHESIS\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
			exit(1);
		}
	}
	else if (tk.tkType == IDENTIFIER) {
		temp->t1 = tk;

		tk_index++; tk = tokens[tk_index];
		return temp;
	}
	else if(tk.tkType == INTEGER){
		temp->t1 = tk;

		tk_index++; tk = tokens[tk_index];
		return temp;
	}
	else{
		printf("\n### <R> Error - Invalid Token Line %d: \nExpected: LEFT_PARENTHESIS\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
		exit(1);
	}
}


//<stats>         ->      <stat>  <mStat>
treeNode* stats(int depth){
	depth++;

	treeNode* temp = nodebuild("<stats>", depth);
	temp->c1 = stat(depth);
	temp->c2 = mStats(depth);

	return temp;
}


//<mStat>       ->      empty |  <stat>  <mStat>
treeNode* mStats(int depth){
	if (tk.tkType == IN_KYD || tk.tkType == OUT_KYD || tk.tkType == BEGIN_KYD ||
		tk.tkType == IFFY_KYD || tk.tkType == LOOP_KYD || tk.tkType == IDENTIFIER) {

		depth++;
		treeNode* temp = nodebuild("<mStats>", depth);

		temp->c1 = stat(depth);
		temp->c2 = mStats(depth);

		return temp;
	}
	else
		return NULL;
}


//<stat>           ->      <in> .  | <out> .  | <block> | <if> .  | <loop> .  | <assign> .
treeNode* stat(int depth){
	depth++;
	treeNode* temp = nodebuild("<stat>", depth);
	
	if (tk.tkType == IN_KYD){ 			//seeks period
		temp->c1 = in(depth);
	}
	else if (tk.tkType == OUT_KYD) {	//seeks period
		temp->c1 = out(depth);
	}
	else if (tk.tkType == BEGIN_KYD) {	//does not seek period
		temp->c1 = block(depth);

		return temp;
	}
	else if (tk.tkType == IFFY_KYD) {	//seeks period
		temp->c1 = if_f(depth);
	}
	else if (tk.tkType == LOOP_KYD) {	//seeks period
		temp->c1 = loop(depth);
	}
	else if (tk.tkType == IDENTIFIER) {	//seeks period
		temp->c1 = assign(depth);
	}
	else {
		printf("\n### <stat> Error - Invalid Token Line %d: \nExpected: IN, OUT, BEGIN, IFFY, LOOP, IDENTIFIER\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);	
		exit(1);
	}

	if (tk.tkType == PERIOD) {
		temp->t1 = tk;

		tk_index++; tk = tokens[tk_index];
		return temp;
	}
	else {
		printf("\n### <stat> Error - Invalid Token Line %d: \nExpected: PERIOD\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
		exit(1);
	}

	return temp;
}


//<in>              ->      in  Identifier
treeNode* in(int depth){
	depth++;
	treeNode* temp = nodebuild("<in>", depth);
	
	if (tk.tkType == IN_KYD){
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index];

		if (tk.tkType == IDENTIFIER) {
			temp->t2 = tk;

			tk_index++; tk = tokens[tk_index];
			return temp;
		}
		else {
			printf("\n### <IN> Error - Invalid Token Line %d: \nExpected: IDENTIFIER\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);		
			exit(1);
		}
	}
	else {
		printf("\n### <IN> Error - Invalid Token Line %d: \nExpected: IN\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);		
		exit(1);
	}
}


//<out>            ->      out <expr>
treeNode* out(int depth){
	depth++;
	treeNode* temp = nodebuild("<out>", depth);
	if (tk.tkType == OUT_KYD) {
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index];
		temp->c1 = expr(depth);
	}
	else{
		printf("\n### <OUT> Error - Invalid Token Line %d: \nExpected: OUT\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
		exit(1);
	}

	return temp;
}


//<if>               ->      iffy [ <expr> <RO> <expr> ] then <stat>
treeNode* if_f(int depth){
	depth++;
	treeNode* temp = nodebuild("<if>", depth);
	if(tk.tkType == IFFY_KYD){
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index];

		if (tk.tkType == LEFT_SQUARE_BRACKET) {
			temp->t2 = tk;
			tk_index++; tk = tokens[tk_index];
			
			temp->c1 = expr(depth);
			temp->c2 = ro(depth);
			temp->c3 = expr(depth);

			if (tk.tkType == RIGHT_SQUARE_BRACKET) {
				temp->t3 = tk;
				tk_index++; tk = tokens[tk_index];

				if (tk.tkType == THEN_KYD) {
					temp->t4 = tk;
					tk_index++; tk = tokens[tk_index];

					temp->c4 = stat(depth);
					return temp;
				}
				else {
					printf("\n### <IF> Error - Invalid Token Line %d: \nExpect\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);				
					exit(1);
				}				
			}
			else {
				printf("\n### <IF> Error - Invalid Token Line %d: \nExpected: RIGHT SQUARE BRACKET\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);				
				exit(1);
			}
		}
		else {
			printf("\n### <IF> Error - Invalid Token Line %d: \nExpected: LEFT SQUARE BRACKET\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);		
			exit(1);
		}
	}
	else{
		printf("\n### <IF> Error - Invalid Token Line %d: \nExpected: IFFY\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);	
		exit(1);
	}
}


//<loop>          ->      loop  [ <expr> <RO> <expr> ]  <stat>
treeNode* loop(int depth){

	depth++;
	treeNode* temp = nodebuild("<loop>", depth);
	if (tk.tkType == LOOP_KYD){ 
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index];
		
		if (tk.tkType == LEFT_SQUARE_BRACKET) {
			temp->t2 = tk;
			tk_index++; tk = tokens[tk_index];

			temp->c1 = expr(depth);
			temp->c2 = ro(depth);
			temp->c3 = expr(depth);

			if (tk.tkType == RIGHT_SQUARE_BRACKET) {
				temp->t3 = tk;

				tk_index++; tk = tokens[tk_index];
				temp->c4 = stat(depth);

				return temp;
			}
			else {
				printf("\n### <LOOP> Error - Invalid Token Line %d: \nExpected: RIGHT SQUARE BRACKET\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);				
				exit(1);
			}
		}
		else {
			printf("\n### <LOOP> Error - Invalid Token Line %d: \nExpected: LEFT SQUARE BRACKET\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);		
			exit(1);
		}
	
	}
	else {
		printf("\n### <LOOP> Error - Invalid Token Line %d: \nExpected: LOOP\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
		
		exit(1);
	}
}


//<assign>       ->      Identifier  = <expr>
treeNode* assign(int depth) {

	depth++;
	treeNode* temp = nodebuild("<assign>", depth);
	if (tk.tkType == IDENTIFIER){
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index];

		if(tk.tkType == EQUAL){
			temp->t2 = tk;
			tk_index++; tk = tokens[tk_index];

			temp->c1 = expr(depth);

			return temp;

		}
		else {
			printf("\n### <ASSIGN> Error - Invalid Token Line %d: \nExpected: LOOP\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
			
			exit(1);
		}
	}
	else {
		printf("\n### <ASSIGN> Error - Invalid Token Line %d: \nExpected: IDENTIFIER\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
		
		exit(1);
	}

}


//<RO>            ->      < | <  <  | >  | >  >  |  ==  |   <  >   
treeNode* ro(int depth){

	depth++;
	treeNode* temp = nodebuild("<ro>", depth);
	if (tk.tkType == LEFT_ANGLE_BRACKET) {
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index];

		if (tk.tkType == LEFT_ANGLE_BRACKET) {
			temp->t2 = tk;
			tk_index++; tk = tokens[tk_index];

			return temp;
		}
		else if (tk.tkType == RIGHT_ANGLE_BRACKET) {
			temp->t2 = tk;
			tk_index++; tk = tokens[tk_index];

			return temp;
		}
		else {

			return temp;
		}
	}
	else if (tk.tkType == RIGHT_ANGLE_BRACKET) {
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index];
		if (tk.tkType == RIGHT_ANGLE_BRACKET) {
			temp->t2 = tk;
			tk_index++; tk = tokens[tk_index];

			return temp;
		}
		else {

			return temp;
		}
	}
	else if (tk.tkType == DOUBLE_EQUAL) {
		temp->t1 = tk;
		tk_index++; tk = tokens[tk_index];

		return temp;
	}
	else {
		printf("\n### <RO> Error - Invalid Token Line %d: \nExpected: LEFT ANGLE BRACKET, RIGHT ANGLE BRACKET, or DOUBLE EQUAL\nReceived %s \n", tk.lineNumber, tokenName[tk.tkType]);
		
		exit(1);
	}

}


//node builder
treeNode* nodebuild(char* ident, int depth) {
	treeNode *new;
	new = malloc(sizeof(treeNode));
	//test();
	strcpy(new->ident, ident);
	new->depth = depth;
	new->c1 = NULL;
	new->c2 = NULL;
	new->c3 = NULL;
	new->c4 = NULL;



	return new;
}


