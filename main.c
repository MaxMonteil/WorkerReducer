#include "main.h"

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

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

        // create N workers
        pthread_t worker_ids[proc_amnt];
        int thread_ids[proc_amnt];
        worker_data = malloc(proc_amnt * sizeof(count_result));
        for (i = 0; i < proc_amnt; i++) {
            thread_ids[i] = i;

            if (0 < pthread_create((worker_ids + i), NULL, worker_func, (thread_ids + i))) {
                printf("There was an error creating the worker threads.\n");
                exit(1);
            }
        }
        // Wait for workers
        for (i = 0; i < proc_amnt; pthread_join(worker_ids[i++], NULL));

        pthread_t reducer_ids[proc_amnt];
        reducer_args reducer_data[REDUCER_AMNT];
        total_results = &(count_result) { 0, 0, 0 };
        for (i = 0; i < REDUCER_AMNT; i++) {
            reducer_data[i] = (reducer_args) { .count = proc_amnt, .target = i };

            if (0 < pthread_create(&reducer_ids[i], NULL, reducer_func, (reducer_data + i))) {
                printf("There was an error creating the threads.\n");
                exit(1);
            }
        }
        // Wait for reducers
        for (i = 0; i < REDUCER_AMNT; pthread_join(reducer_ids[i++], NULL));

        printf("total.CMPS %d\n", total_results->CMPS);
        printf("total.CCE %d\n", total_results->CCE);
        printf("total.ECE %d\n", total_results->ECE);

        // CLEANUP
        free(worker_data);
        // free(total_results);

        exit(0);
    } else { // split the text file
        char part_len_str[MAX_INT];
        snprintf(part_len_str, MAX_INT, "%d", (file_len(IN_NAME) / proc_amnt));
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

void map_file(char *file_name, int *file_map, int file_len, int part_count) {
    /*
     * Because the file lines are of unequal length I can't just fseek to
     * a position obtained from dividing the full length by the part count
     * it might place the cursor in the middle of a word.
     *
     * map_file will fill an array with the byte offset marking the start
     * of each file section.
    */
    int ch, i = 0, lines = 0;
    FILE *fp;

    int part_len = file_len / part_count;

    if ((fp = fopen(file_name, "r")) == NULL) {
        printf("Error opening file %s\n", file_name);
        exit(1);
    }
    while (EOF != (ch = fgetc(fp)))
        if (ch == '\n') {
            if (lines % part_len == 0) *(file_map + i++) = ftell(fp);
            ++lines;
        }
    *(file_map + i) = ftell(fp);

    fclose(fp);
}
