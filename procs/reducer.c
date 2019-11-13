#include "reducer.h"

#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv) {
    long proc_amnt = strtol(argv[1], NULL, 10);
    long shmem_ids[proc_amnt];
    create_n_shmem(proc_amnt, IN_NAME, shmem_ids, 0666);

    count_result *results[proc_amnt];
    attach_n_shmem(proc_amnt, shmem_ids, results);

    count_result total = { 0, 0, 0, 0 };
    for (int i = 0; i < proc_amnt; ++i) {
        while (results[i]->status != 999);

        if (0 == strcmp(argv[2], "CMPS"))
            total.CMPS += results[i]->CMPS;
        else if (0 == strcmp(argv[2], "CCE"))
            total.CCE += results[i]->CCE;
        else
            total.ECE += results[i]->ECE;
    }

    detach_n_shmem(proc_amnt, results);

    return 0;
}
