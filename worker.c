#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "structs.h"
#include "definitions.h"

int compare (char *word, char **strings, count_result *result);

int main (int argc, char **argv) {
    char *strings[] = { "CMPS", "CCE", "ECE" };
    char FMT_OUT_NAME[FMT_OUT_NAME_LEN];
    FILE *fp;

    snprintf(FMT_OUT_NAME, FMT_OUT_NAME_LEN, "%s%s", OUT_NAME, argv[2]);
    if ((fp = fopen(FMT_OUT_NAME, "r")) == NULL) {
        printf("Unable to open file %s\n", FMT_OUT_NAME);
        exit(1);
    }

    //                                  first char of string converted to int
    int shmem_id = shmget(ftok(IN_NAME, **(argv + 2) - '0'), sizeof(count_result), 0666);
    count_result *result = (count_result *) shmat(shmem_id, NULL, 0);

    int c, i = 0;
    char *word = (char *) malloc(5 * sizeof(char));
    while ((c = fgetc(fp)) != EOF) {
        if (c == ' ' || c == '\n') {
            *(word + i++) = '\0';
            i = compare(word, strings, result);
        } else {
            *(word + i++) = (char) c;
        }
    }
    result->status = 999;

    fclose(fp);
    free(word);
    shmdt(result);

    return 0;
}

int compare (char *word, char **strings, count_result *result) {
    if (strcmp(word, *strings) == 0)
        result->CMPS += 1;
    else if (strcmp(word, *(strings + 1)) == 0)
        result->CCE += 1;
    else
        result->ECE += 1;

    return 0;
}
