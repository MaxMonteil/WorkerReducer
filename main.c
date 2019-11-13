#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char **argv) {
    int i, proc_amnt = 4;
    clock_t start, end;

    // cast command line arg as integer
    char *p;
    errno = 0;
    if (argc > 1) {
        proc_amnt = strtol(argv[1], &p, 10);
        if (*p != '\0' || errno != 0)
            return 1;
    }

    start = clock();
    printf("Hello, World!\n");
    for (i = 0; i < proc_amnt; i++) printf("%d ", i);
    printf("\n");
    end = clock();

    printf("Elapsed time: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
