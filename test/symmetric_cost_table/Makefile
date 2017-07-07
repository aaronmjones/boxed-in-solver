SUBDIRS=print_test

all: $(SUBDIRS)

$(SUBDIRS):
	make -C $@

clean:
	make -C print_test clean
re: clean all

.PHONY: all clean re $(SUBDIRS)
