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

    // cast command line arg as integer
    char *p;
    errno = 0;
    if (argc > 1) {
        proc_amnt = strtol(argv[1], &p, 10);
        if (*p != '\0' || errno != 0)
            return 1;
    }

    // Analyze input file
    int file_map[proc_amnt + 1];
    analyze_file(IN_NAME, file_map, proc_amnt);

    // Create N workers
    // Allocating global shared variable
    worker_data = malloc(proc_amnt * sizeof(count_result));
    if (worker_data == NULL) {
        printf("Error: Unable to allocate memory.\n");
        exit(1);
    }

    pthread_t worker_ids[proc_amnt];
    worker_args worker_arguments[proc_amnt];
    for (i = 0; i < proc_amnt; i++) {
        *(worker_data + i) = (count_result) { .CMPS = 0, .CCE = 0, .ECE = 0 };
        worker_arguments[i] = (worker_args) { .id = i, .start = file_map[i], .end = file_map[i + 1] };

        if (0 < pthread_create((worker_ids + i), NULL, worker_func, (worker_arguments + i))) {
            printf("There was an error creating the worker threads.\n");
            exit(1);
        }
    }
    for (i = 0; i < proc_amnt; pthread_join(worker_ids[i++], NULL)); // Wait for workers

    // Create 3 reducers
    // Allocating global shared variable
    total_results = malloc(sizeof(count_result));
    if (total_results == NULL) {
        printf("Error: Unable to allocate memory.\n");
        exit(1);
    }

    pthread_t reducer_ids[proc_amnt];
    reducer_args reducer_data[REDUCER_AMNT];
    *total_results = (count_result) { 0, 0, 0 };
    for (i = 0; i < REDUCER_AMNT; i++) {
        reducer_data[i] = (reducer_args) { .count = proc_amnt, .target = i };

        if (0 < pthread_create(&reducer_ids[i], NULL, reducer_func, (reducer_data + i))) {
            printf("There was an error creating the threads.\n");
            exit(1);
        }
    }
    for (i = 0; i < REDUCER_AMNT; pthread_join(reducer_ids[i++], NULL)); // Wait for reducers

    printf("total.CMPS %d\n", total_results->CMPS);
    printf("total.CCE %d\n", total_results->CCE);
    printf("total.ECE %d\n", total_results->ECE);

    // CLEANUP
    free(worker_data);
    free(total_results);

    return 0;
}

void analyze_file (char *file_name, int *file_map, int part_count) {
    /* Wrapper function to avoid opening and closing the file twice */
    FILE *fp;

    if ((fp = fopen(file_name, "r")) == NULL) {
        printf("Error opening file %s\n", file_name);
        exit(1);
    }

    map_file(fp, file_name, file_map, file_len(fp, file_name), part_count);

    fclose(fp);
}

int file_len (FILE *fp, char *file_name) {
    int ch, lines = 0;

    // count number of lines in file fp
    rewind(fp);
    while (EOF != (ch = fgetc(fp)))
        if (ch == '\n')
            ++lines;

    return lines;
}

void map_file (FILE *fp, char *file_name, int *file_map, int file_len, int part_count) {
    /*
     * Because the file lines are of unequal length I can't just fseek to
     * a position obtained from dividing the full length by the part count
     * it might place the cursor in the middle of a word.
     *
     * map_file will fill an array with the byte offset marking the start
     * of each file section.
    */
    int ch, i = 1, lines = 0;

    int part_len = file_len / part_count;
    if (part_len <= 0) exit(1);

    rewind(fp);
    *file_map = 0;
    while (EOF != (ch = fgetc(fp)))
        if (ch == '\n')
            if (++lines % part_len == 0)
                *(file_map + i++) = ftell(fp);
}
