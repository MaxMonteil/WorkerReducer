#ifndef _MULTI_MAIN_H_INCLUDED
#define _MULTI_MAIN_H_INCLUDED

#include "worker.h"
#include "reducer.h"
#include "definitions.h"
#include "shared.h"
#include <stdio.h>

#define REDUCER_AMNT 3

void analyze_file (char*, int*, int);

#endif // _MULTI_MAIN_H_INCLUDED
