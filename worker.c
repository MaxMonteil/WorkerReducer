#include "worker.h"

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

void *worker_func (void *thread_id) {
    char *targets[] = { "CMPS", "CCE", "ECE" };
    char FMT_OUT_NAME[FMT_OUT_NAME_LEN];
    FILE *fp;

    // find and open proper file parition
    snprintf(FMT_OUT_NAME, FMT_OUT_NAME_LEN, "%s%d", OUT_NAME, *((int *) thread_id));
    if ((fp = fopen(FMT_OUT_NAME, "r")) == NULL) {
        printf("Unable to open file %s\n", FMT_OUT_NAME);
        exit(1);
    }

    int c, i = 0;
    char *word = (char *) malloc(MAX_TARGET_LEN * sizeof(char));
    while ((c = fgetc(fp)) != EOF) {
        if (c == ' ' || c == '\n') {
            *(word + i++) = '\0';
            compare(word, targets, (worker_data + *((int *) thread_id)));
            i = 0;
        } else {
            *(word + i++) = (char) c;
        }
    }

    fclose(fp);
    free(word);

    pthread_exit(NULL);
}

void compare (char *word, char **strings, count_result *result) {
    if (strcmp(word, *strings) == 0)
        result->CMPS += 1;
    else if (strcmp(word, *(strings + 1)) == 0)
        result->CCE += 1;
    else
        result->ECE += 1;
}
