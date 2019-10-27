CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall -o
BIN_DIR = ./bin/
MAIN_OUTPUT = main
WORKER_OUTPUT = worker
REDUCER_OUTPUT = reducer
OUTPUT = $(MAIN_OUTPUT) $(WORKER_OUTPUT) $(REDUCER_OUTPUT)

run: $(OUTPUT)

main: WorkerReducer.o proc_utils.o
	$(CC) $(LFLAGS) $(BIN_DIR)$(MAIN_OUTPUT) $^

worker: worker.o
	$(CC) $(LFLAGS) $(BIN_DIR)$(WORKER_OUTPUT) $^

reducer: reducer.o
	$(CC) $(LFLAGS) $(BIN_DIR)$(REDUCER_OUTPUT) $^

WorkerReducer.o: WorkerReducer.c proc_utils.h definitions.h
	$(CC) $(CFLAGS) $<

worker.o: worker.c structs.h definitions.h
	$(CC) $(CFLAGS) $<

reducer.o: reducer.c
	$(CC) $(CFLAGS) $<

proc_utils.o: proc_utils.c proc_utils.h structs.h
	$(CC) $(CFLAGS) $<

wipe:
	@rm -rf *.o $(BIN_DIR)* output*

.PHONY: clean
clean:
	@rm -rf output*
