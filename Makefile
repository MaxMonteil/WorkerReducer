CC=gcc
CFLAGS=-I. -Wall

MAIN_DEPS = main.c main.h $(PROC_DEPS)
WORKER_DEPS = worker.c worker.h structs.h
REDUCER_DEPS = reducer.c reducer.h $(PROC_DEPS)
PROC_DEPS =  proc_utils.c proc_utils.h definitions.h

run: main worker reducer

main: main.o proc_utils.o
	$(CC) -o run_procs $^ $(CFLAGS)

main.o: $(MAIN_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

worker: worker.o
	$(CC) -o $@ $^ $(CFLAGS)

worker.o: $(WORKER_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

reducer: reducer.o proc_utils.o
	$(CC) -o $@ $^ $(CFLAGS)

reducer.o: $(REDUCER_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

proc_utils.o: $(PROC_DEPS) structs.h
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	@rm -f *.o run_procs worker reducer output*

.PHONY: fresh
fresh:
	@rm -f output*
