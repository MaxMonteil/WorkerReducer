CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall -o
MAIN_OUTPUT = main
WORKER_OUTPUT = worker
REDUCER_OUTPUT = reducer
OUTPUT = $(MAIN_OUTPUT) $(WORKER_OUTPUT) $(REDUCER_OUTPUT)

run: $(OUTPUT)

main: WorkerReducer.o proc_utils.o
	$(CC) $(LFLAGS) $(MAIN_OUTPUT) $^

worker: worker.o
	$(CC) $(LFLAGS) $(WORKER_OUTPUT) $^

reducer: reducer.o
	$(CC) $(LFLAGS) $(REDUCER_OUTPUT) $^

WorkerReducer.o: WorkerReducer.c proc_utils.h definitions.h
	$(CC) $(CFLAGS) $<

worker.o: worker.c structs.h definitions.h
	$(CC) $(CFLAGS) $<

reducer.o: reducer.c
	$(CC) $(CFLAGS) $<

proc_utils.o: proc_utils.c proc_utils.h structs.h
	$(CC) $(CFLAGS) $<

wipe:
	@rm -rf *.o $(OUTPUT) output*

.PHONY: clean
clean:
	@rm -rf output*
