#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

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

    printf("Hello, World!\n");
    for (i = 0; i < proc_amnt; i++) printf("%d ", i);
    printf("\n");

    return 0;
}
