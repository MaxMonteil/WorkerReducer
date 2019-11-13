#ifndef _MAIN_H_INCLUDED
#define _MAIN_H_INCLUDED

#include "worker.h"
#include "reducer.h"
#include "definitions.h"
#include "shared.h"
#include <stdio.h>

#define REDUCER_AMNT 3

void analyze_file (char*, int*, int);
int file_len (FILE*, char*);
void map_file (FILE*, char*, int*, int, int);

#endif // _MAIN_H_INCLUDED
