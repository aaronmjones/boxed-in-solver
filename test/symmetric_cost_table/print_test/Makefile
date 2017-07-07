


OBJECTSDIR=../bin

EXE=$(OBJECTSDIR)/print_test

CXXFLAGS += \
	-Wall \
	-std=c++11 \
	-std=gnu++11 \
	-rdynamic
CXXFLAGS+=-m64

ifeq ($(DEBUG),1)
CXXFLAGS+= -g -Wall
else
CXXFLAGS+=-O3
endif

CXXSOURCES += \
	main.cc

INCPATH += \
	-I. \
	-I../../../libboxedin/include \
	-I../../../../googletest/googletest/include

LIBPATH += \
	-L../../../libboxedin \
	-L../../../../googletest/googletest/make

LIBS += \
	-lstdc++ \
	-lboxedin \
	-lgtest_main \
	-lpthread

CXXDEPEND := $(CXXSOURCES:%.cc=%.d)
CXXOBJECTS := $(CXXSOURCES:%.cc=%.o)

all: $(EXE)

clean:
	rm -f $(EXE)
	rm -f $(CXXOBJECTS)
	rm -f $(CXXDEPEND)

re:
	make clean
	make all

# The “-” means do not display error or warning if target cannot
# be remade.
ifneq ($(MAKECMDGOALS),clean)
-include $(CXXDEPEND)
endif

# -MM: generate include dependencies
# -MP: CPP phony target
# -MT: target string
# -MF: file to write dependencies to
# $@: target that caused recipe to execute
# $<: the first prerequisite
$(CXXDEPEND): %.d: %.cc
	$(CXX) $(CXXFLAGS) $(INCPATH) -MM -MP -MT '$(@:%.d=%.o) $@' -MF $@ $<

#%.o: %.cc
$(CXXOBJECTS): %.o: %.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $(LIBPATH) $(LIBS) $< -o $@

$(EXE): $(CXXOBJECTS) ../../../libboxedin/libboxedin.a
	$(CXX) $(CXXFLAGS) $(INCPATH) $(LIBPATH) $(CXXOBJECTS) $(LIBS) -o $@

