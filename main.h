#ifndef _MAIN_H_INCLUDED
#define _MAIN_H_INCLUDED

#include "worker.h"
#include "reducer.h"
#include "definitions.h"
#include "shared.h"

#define REDUCER_AMNT 3

int file_len (char*);
void map_file(char*, int*, int, int);

#endif // _MAIN_H_INCLUDED
