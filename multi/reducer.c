#include "reducer.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

void *reducer_func (void *args) {
    for (int i = 0; i < ((reducer_args *) args)->count; i++) {
        if (CMPS == ((reducer_args *) args)->target)
            total_results->CMPS += (worker_data + i)->CMPS;
        else if (CCE == ((reducer_args *) args)->target)
            total_results->CCE += (worker_data + i)->CCE;
        else
            total_results->ECE += (worker_data + i)->ECE;
    }

    pthread_exit(NULL);
}
