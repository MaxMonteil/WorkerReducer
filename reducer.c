#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "proc_utils.h"
#include "definitions.h"

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

    if (0 == strcmp(argv[2], "CMPS"))
        printf("total.CMPS %ld\n", total.CMPS);
    else if (0 == strcmp(argv[2], "CCE"))
        printf("total.CCE %ld\n", total.CCE);
    else
        printf("total.ECE %ld\n", total.ECE);

    detach_n_shmem(proc_amnt, results);

    return 0;
}
