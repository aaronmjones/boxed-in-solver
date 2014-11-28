################################################################################
# Makefile for BoxedInSolver
#
################################################################################
CXX=$(CROSS)g++
AR=ar

LIB=libboxedin.a

CXXFLAGS += \
	-Wall \
	-std=c++11 \
	-std=gnu++11 \
	-rdynamic
CXXFLAGS+=-O3
CXXFLAGS+=-m64

CXXSOURCES += \
	src/BoxedInNode.cc \
	src/LevelState.cc \
	src/boxedinio.cc \
	src/astar.cc \
	src/memusage.cc

INCPATH += \
	-Iinclude

#LIBPATH +=
#LIBS += -lstdc++

CXXDEPEND := $(CXXSOURCES:%.cc=%.d)
CXXOBJECTS := $(CXXSOURCES:%.cc=%.o)

all: $(LIB)

clean:
	rm -f $(LIB)
	rm -f $(CXXOBJECTS)
	rm -f $(CXXDEPEND)

re: clean all

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

$(CXXOBJECTS): %.o: %.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $(LIBPATH) $(LIBS) $< -o $@

$(LIB): $(CXXOBJECTS)
	$(AR) rvs $@ $(CXXOBJECTS)

