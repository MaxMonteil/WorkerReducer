#include <sys/types.h>
#include "structs.h"

void create_n_shmem (int n, char *path, long *shmem_ids);
void attach_n_shmem (int n, long *shmem_ids, count_result **result);
void detach_n_shmem (int n, count_result **results);
void remove_n_shmem (int n, long* shmem_ids);
void create_n_procs (int n, pid_t *container, char *path, char *name);
