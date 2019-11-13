CC=gcc
CFLAGS=-I. -Wall

CMN_DEPS = shared.c shared.h definitions.h structs.h
MAIN_DEPS = main.c main.h $(CMN_DEPS)
WORKER_DEPS = worker.c worker.h $(CMN_DEPS)
REDUCER_DEPS = reducer.c reducer.h $(CMN_DEPS)

main: main.o worker.o reducer.o shared.o
	$(CC) -o run_multi $^ $(CFLAGS) -lpthread

main.o: $(MAIN_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

worker: worker.o
	$(CC) -o $@ $^ $(CFLAGS)

worker.o: $(WORKER_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

reducer: reducer.o
	$(CC) -o $@ $^ $(CFLAGS)

reducer.o: $(REDUCER_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shared.o: $(CMN_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o run_multi
