#ifndef NODE_H
#define NODE_H

#include "token.h"

#define LENGTH 10

typedef struct treeNode {
    int key;
    int depth;
    char ident[LENGTH];

    Token t1;
    Token t2;
    Token t3;
    Token t4;
    Token t5;

    struct treeNode *c1;
    struct treeNode *c2;
    struct treeNode *c3;
    struct treeNode *c4;
 

} treeNode;

treeNode* tree;
#endif