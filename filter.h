#ifndef FILTER_H
#define FILTER_H



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WHOLELINE1 70


void filter(FILE*, const char*);
bool comment_seeker(char*, int, bool);
bool comment_removal(char*, int, bool, char);


#endif

