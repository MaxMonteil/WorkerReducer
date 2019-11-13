#ifndef _PROCS_UTILS_H_INCLUDED
#define _PROCS_UTILS_H_INCLUDED

#include <sys/types.h>
#include "structs.h"

void create_n_shmem (int, char*, long*, int);
void attach_n_shmem (int, long*, count_result**);
void detach_n_shmem (int, count_result**);
void remove_n_shmem (int, long*);
void create_n_workers (int, pid_t*, char*, char*);
void create_n_reducers (int, pid_t*, char*, char*, char*, char**);

#endif // _PROCS_UTILS_H_INCLUDED
