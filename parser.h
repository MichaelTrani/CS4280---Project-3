#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "token.h"
#include "node.h"


treeNode* parser();
treeNode* nodebuild(char*, int);
treeNode* program() ;

treeNode* block(int) ;
treeNode* vars(int) ;
treeNode* expr(int) ;
treeNode* N(int) ;
treeNode* A(int) ;
treeNode* M(int) ;
treeNode* R(int) ;
treeNode* stats(int) ;
treeNode* mStats(int) ;
treeNode* stat(int) ;
treeNode* in(int) ;
treeNode* out(int) ;
treeNode* if_f(int) ;
treeNode* loop(int) ;
treeNode* assign(int) ;
treeNode* ro(int) ;


#endif //PARSER_H