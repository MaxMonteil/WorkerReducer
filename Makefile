run: multi procs

.PHONY: multi
multi:
	$(MAKE) -C multi

.PHONY: procs
procs:
	$(MAKE) -C procs

.PHONY: clean
clean: clean_multi clean_procs

clean_multi:
	$(MAKE) -C multi clean

clean_procs:
	$(MAKE) -C procs clean

.PHONY: fresh
fresh:
	$(MAKE) -C procs fresh
