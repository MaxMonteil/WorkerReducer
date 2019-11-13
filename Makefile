CC=gcc
CFLAGS=-I. -Wall

run: main multi procs

main: main.o
	$(CC) -o main $^ $(CFLAGS)

main.o: main.c main.h
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: multi
multi:
	$(MAKE) -C multi

.PHONY: procs
procs:
	$(MAKE) -C procs

.PHONY: clean
clean: clean_multi clean_procs
	rm -f main.o main output*

clean_multi:
	$(MAKE) -C multi clean

clean_procs:
	$(MAKE) -C procs clean

.PHONY: fresh
fresh:
	rm -f output* && $(MAKE) -C procs fresh
