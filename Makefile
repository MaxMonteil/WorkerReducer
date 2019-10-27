CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall -o
MAIN_OUTPUT = main
WORKER_OUTPUT = worker
REDUCER_OUTPUT = reducer
OUT_EXT = .out
OUTPUT = $(MAIN_OUTPUT) $(WORKER_OUTPUT) $(REDUCER_OUTPUT)

run: $(OUTPUT)

$(MAIN_OUTPUT): WorkerReducer.o proc_utils.o
	$(CC) $(LFLAGS) $@$(OUT_EXT) $^

$(WORKER_OUTPUT): worker.o
	$(CC) $(LFLAGS) $@$(OUT_EXT) $^

$(REDUCER_OUTPUT): reducer.o proc_utils.o
	$(CC) $(LFLAGS) $@$(OUT_EXT) $^

WorkerReducer.o: WorkerReducer.c proc_utils.h definitions.h
	$(CC) $(CFLAGS) $<

worker.o: worker.c structs.h definitions.h
	$(CC) $(CFLAGS) $<

reducer.o: reducer.c proc_utils.h definitions.h
	$(CC) $(CFLAGS) $<

proc_utils.o: proc_utils.c proc_utils.h structs.h
	$(CC) $(CFLAGS) $<

wipe:
	@rm -rf *.o *$(OUT_EXT) output*

.PHONY: clean
clean:
	@rm -rf output*
