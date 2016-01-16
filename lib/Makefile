# Makefile -- GNU Makefile for making tst libraries - assumes that objectfiles have already been built
#                   libraries
#

include ../MakeVars


lib: libtrk$(TRK_VERSION).so

libtrk$(TRK_VERSION).so : $(wildcard ./objs/*.o)
	$(CXX) $(LIBFLAGS) -o $@ $(wildcard ./objs/*.o)

clean: 
	$(RM) libtrk$(TRK_VERSION).so
	$(RM) ./objs/*.o

