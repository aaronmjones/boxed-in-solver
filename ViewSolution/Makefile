################################################################################
# Makefile for ViewSolution
#
################################################################################
CXX=$(CROSS)g++

OBJECTSDIR=../bin

EXE=$(OBJECTSDIR)/ViewSolution

CXXFLAGS += \
	-Wall \
	-std=c++11 \
	-std=gnu++11 \
	-rdynamic
CXXFLAGS+=-m64

CXXSOURCES += \
	ViewSolution.cc

INCPATH += \
	-I../libboxedin/include \
	-I../boost_1_64_0

LIBPATH += \
	-L../libboxedin

LIBS += \
	-lstdc++ \
	-lboxedin

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

$(EXE): $(CXXOBJECTS)
	$(CXX) $(CXXFLAGS) $(INCPATH) $(LIBPATH) $(CXXOBJECTS) $(LIBS) -o $@
