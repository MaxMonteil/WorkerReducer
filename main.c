#include <sys/wait.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "WorkerReducer.h"

int main (int argc, char **argv) {
    int i, proc_amnt = 4;
    pid_t pid;

    // cast command line arg as integer
    char *p;
    errno = 0;
    if (argc > 1) {
        proc_amnt = strtol(argv[1], &p, 10);
        if (*p != '\0' || errno != 0)
            return 1;
    }

    if ((pid = fork()) == -1) {
        printf("Error in fork\n");
        return 1;
    }

    if (pid > 0) { // parent process
        waitpid(pid, NULL, 0); // wait for split to be done

        long shmem_ids[proc_amnt];
        create_n_shmem(proc_amnt, IN_NAME, shmem_ids, IPC_CREAT | 0666);

        // create N workers
        pid_t worker_pids[proc_amnt];
        create_n_workers(proc_amnt, worker_pids, "./worker.out", "worker");

        // create 3 reducers
        pid_t reducer_pids[REDUCER_AMNT];
        char proc_amnt_str[12];
        char *targets[REDUCER_AMNT] = { "CMPS", "CCE", "ECE" };
        snprintf(proc_amnt_str, 12, "%d", proc_amnt);
        create_n_reducers(REDUCER_AMNT, reducer_pids, "./reducer.out", "reducer", proc_amnt_str, targets);

        for (i = 0; i < proc_amnt; i++) waitpid(worker_pids[i], NULL, 0);
        for (i = 0; i < REDUCER_AMNT; i++) waitpid(reducer_pids[i], NULL, 0);

        remove_n_shmem(proc_amnt, shmem_ids);
        exit(0);
    } else { // split the text file
        char part_len_str[12];
        snprintf(part_len_str, 12, "%d", (file_len(IN_NAME) / proc_amnt));
        execlp("split", "split", "-l", part_len_str, "-a", "1", "-d", IN_NAME, OUT_NAME, (char *) NULL);
        exit(0);
    }

    return 0;
}

int file_len (char *file_name) {
    int ch, lines = 0;
    FILE *fp;

    // count number of lines in file fp
    if ((fp = fopen(file_name, "r")) == NULL) {
        printf("Error opening file %s\n", file_name);
        exit(1);
    }
    while (EOF != (ch = fgetc(fp)))
        if (ch == '\n')
            ++lines;
    fclose(fp);

    return lines;
}
