#include "main.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main (int argc, char **argv) {
    int i, err;
    char *test_values[NUM_OF_TESTS] = { "1", "4", "8" };
    clock_t start, end;
    pid_t test_process_ids[NUM_OF_TESTS];
    double process_times[NUM_OF_TESTS], threaded_times[NUM_OF_TESTS];

    printf("Testing multiple processes vs multiple threads for ");
    for (i = 0; i < NUM_OF_TESTS; i++) printf("%s ", test_values[i]);
    printf("splits.\n\n");

    // Run multiprocess tests
    for (i = 0; i < NUM_OF_TESTS; i++) {
        if ((test_process_ids[i] = fork()) == -1) {
            printf("Error creating fork\n");
            exit(1);
        }

        printf("Testing multiple processes and %s splits", test_values[i]);
        if (test_process_ids[i] == 0) { // child process
            err = execlp(PROCS_PATH, PROCS_NAME, test_values[i], NULL);
            printf("error %d\n", err);
            exit(0);
        } else { // parent process
            start = clock();
            waitpid(test_process_ids[i], NULL, 0);
            end = clock();
            printf("\t\tDone!\n");
            process_times[i] = (double)(end - start) / CLOCKS_PER_SEC;
        }
    }

    // Run multithreaded tests
    for (i = 0; i < NUM_OF_TESTS; i++) {
        if ((test_process_ids[i] = fork()) == -1) {
            printf("Error creating fork\n");
            exit(1);
        }

        printf("Testing multiple threads and %s splits", test_values[i]);
        if (test_process_ids[i] == 0) { // child process
            execlp(MULTI_PATH, MULTI_NAME, test_values[i], NULL);
            exit(0);
        } else { // parent process
            start = clock();
            waitpid(test_process_ids[i], NULL, 0);
            end = clock();
            printf("\t\tDone!\n");
            threaded_times[i] = (double)(end - start) / CLOCKS_PER_SEC;
        }
    }

    for (i = 0; i < NUM_OF_TESTS; i++) {
        printf("\nTime taken for %s splits:\n", test_values[i]);
        printf("\tProcesses: %f\n", process_times[i]);
        printf("\tThreading: %f\n\n", threaded_times[i]);
    }

    return 0;
}
