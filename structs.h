#ifndef _STRUCTS_H_INCLUDED
#define _STRUCTS_H_INCLUDED

typedef enum target { CMPS, CCE, ECE } Target;

typedef struct CountResult {
    int CMPS;
    int CCE;
    int ECE;
} count_result;

typedef struct WorkerArgs {
    int id;
    int start;
    int end;
} worker_args;

typedef struct ReducerArgs {
    int count;
    Target target;
} reducer_args;

#endif // _STRUCTS_H_INCLUDED
