#include "worker.h"

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

void *worker_func (void *args) {
    char *targets[] = { "CMPS", "CCE", "ECE" };
    FILE *fp;

    // find and open proper file partion
    if ((fp = fopen(IN_NAME, "r")) == NULL) {
        printf("Unable to open file %s\n", IN_NAME);
        exit(1);
    }
    fseek(fp, ((worker_args *) args)->start, SEEK_SET);

    int c, i = 0;
    int pos = ((worker_args *) args)->start;
    char *word = (char *) malloc(MAX_TARGET_LEN * sizeof(char));
    if (word == NULL) {
        printf("Error: Unable to allocate memory.\n");
        exit(1);
    }

    while ((c = fgetc(fp)) != EOF && ((worker_args *) args)->end > pos++) {
        if (c != ' ' && c != '\n') {
            *(word + i++) = (char) c;
            continue;
        }

        if (i == 0) continue; // ignore spaces at the end of the lines

        *(word + i) = '\0';
        compare(word, targets, worker_data + ((worker_args *) args)->id);
        i = 0;
    }

    fclose(fp);
    free(word);

    pthread_exit(NULL);
}

void compare (char *word, char **strings, count_result *result) {
    if (strcmp(word, *strings) == 0)
        ++result->CMPS;
    else if (strcmp(word, *(strings + 1)) == 0)
        ++result->CCE;
    else {
        ++result->ECE;
    }
}
