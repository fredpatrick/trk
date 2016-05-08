

include ../MakeVars

THIS_DIR = $(PRJDIR)/cpp
OBJ_PREFIX = $(PRJDIR)/lib/objs/

SUBDIRS = 

#********** define prg tagets (i.e. .cc's for execuatables not lib members)
PRGCC := listConfig.cpp trkDriver.cpp trkClear.cpp trkSwitcher.cpp trkBlocker.cpp

ALLCC = $(wildcard *.cpp)
include $(MAKE_DEPS)

LIBCC:=  $(filter-out $(PRGCC), $(ALLCC) )

all: obj lib prg

#*************** compile libAos components
# NOTE: LIBOBJS is target of static pattern ruledefined in MakeRules

LIBOBJS = $(patsubst %.cpp,$(OBJ_PREFIX)%.o, $(LIBCC))
PRGOBJS = $(patsubst %.cpp,%.o, $(PRGCC))

obj: $(LIBOBJS)

lib: 
	$(MAKE) -C ../lib lib || exit 1; done

PRGS  := $(patsubst %.cpp, %.exe, $(PRGCC) )

#******************* compile and link executables
prg: $(PRGS)
	for i in $(SUBDIRS); do $(MAKE) -C $$i prg || exit 1; done 

clean:
	rm -f deps/*.d *.o *.exe
	for i in $(SUBDIRS); do $(MAKE) -C $$i clean || exit 1; done 

include ../MakeRules
