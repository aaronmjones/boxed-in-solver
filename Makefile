MAKEDIRS= \
	libboxedin \
	BoxedInSolver \
	ViewSolution

all:
	$(foreach dir,$(MAKEDIRS),make -C $(dir) all &&) true

re:
	$(foreach dir,$(MAKEDIRS),make -C $(dir) re &&) true

clean:
	$(foreach dir,$(MAKEDIRS),make -C $(dir) clean &&) true
