
GTEST_DIR=/home/ajones/development/googletest/googletest

LIBGTEST=libgtest.a
GTESTALL=gtest-all.o
SUBDIRS=\
	symmetric_cost_table \
	encoded_path

all: $(LIBGTEST) $(SUBDIRS)

$(LIBGTEST):
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv $(LIBGTEST) $(GTESTALL)

re: clean all

clean:
	rm -rf $(LIBGTEST) $(GTESTALL)
$(SUBDIRS):
	make -C $@

.PHONY: all clean re $(SUBDIRS)
