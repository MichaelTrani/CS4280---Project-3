#ifndef FSATABLE_H
#define FSATABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define states 22
#define elements 24

int FSAtable[states][elements];


void buildFSAtable();


#endif