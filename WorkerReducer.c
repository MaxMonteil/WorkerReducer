#include <sys/wait.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "proc_utils.h"
#include "definitions.h"

#define REDUCER_AMNT 3

int file_len (char *file_name);

int main (int argc, char **argv) {
    int i, split = 4;
    pid_t pid;

    // cast command line arg as integer
    char *p;
    errno = 0;
    if (argc > 1) {
        split = strtol(argv[1], &p, 10);
        if (*p != '\0' || errno != 0)
            return 1;
    }

    if ((pid = fork()) == -1) {
        printf("Error in fork\n");
        return 1;
    }

    if (pid > 0) { // parent process
        waitpid(pid, NULL, 0); // wait for split to be done

        long shmem_ids[split];
        create_n_shmem(split, IN_NAME, shmem_ids);

        count_result *results[split];
        attach_n_shmem(split, shmem_ids, results);

        // create N processes
        pid_t worker_pids[split], reducer_pids[REDUCER_AMNT];

        char *worker_args[] = { "./worker", "worker", "0" };
        char *reducer_args[] = { "./reducer", "reducer", "0" };

        create_n_procs(split, worker_pids, worker_args);
        create_n_procs(REDUCER_AMNT, reducer_pids, reducer_args);

        for (i = 0; i < split; i++) waitpid(worker_pids[i], NULL, 0);
        for (i = 0; i < REDUCER_AMNT; i++) waitpid(reducer_pids[i], NULL, 0);

        detach_n_shmem(split, results);
        remove_n_shmem(split, shmem_ids);
        exit(0);
    } else { // split the text file
        char part_len_str[12];
        snprintf(part_len_str, 12, "%d", (file_len(IN_NAME) / split));
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
