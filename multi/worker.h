#ifndef _MULTI_WORKER_H_INCLUDED
#define _MULTI_WORKER_H_INCLUDED

#include "structs.h"
#include "definitions.h"
#include "shared.h"

#define MAX_TARGET_LEN 5

void *worker_func (void*);
void compare (char*, char**, count_result*);

#endif // _MULTI_WORKER_H_INCLUDED
