#include "proc_utils.h"

#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void create_n_shmem (int n, char *path, long *shmem_ids, int flag) {
    for (int i = 0; i < n; ++i) {
        *(shmem_ids + i) = shmget(ftok(path, i), sizeof(count_result), flag);
    }
}

void attach_n_shmem (int n, long *shmem_ids, count_result **results) {
    for (int i = 0; i < n; ++i)
        *(results + i) = (count_result *) shmat(*(shmem_ids + i), NULL, 0);
}

void detach_n_shmem (int n, count_result **results) {
    for (int i = 0; i < n; ++i)
        shmdt(*(results + i));
}

void remove_n_shmem (int n, long* shmem_ids) {
    for (int i = 0; i < n; ++i)
        shmctl(*(shmem_ids + i), IPC_RMID, NULL);
}

void create_n_workers (int n, pid_t *container, char *path, char *name) {
    for (int i = 0; i < n; ++i) {
        if ((*(container + i) = fork()) == -1) {
            printf("Error in fork\n");
            exit(1);
        }

        if (*(container + i) == 0) {
            char proc_num[MAX_INT];
            snprintf(proc_num, MAX_INT, "%d", i); // pass the process count
            execlp(path, name, proc_num, NULL);
            exit(0);
        }
    }
}

void create_n_reducers (int n, pid_t *container, char *path, char *name, char *proc_amnt, char **targets) {
    for (int i = 0; i < n; ++i) {
        if ((*(container + i) = fork()) == -1) {
            printf("Error in fork\n");
            exit(1);
        }

        if (*(container + i) == 0) {
            execlp(path, name, proc_amnt, *(targets + i), NULL);
            exit(0);
        }
    }
}
