# Makefile -- GNU Makefile for making tst libraries - assumes that objectfiles have already been built
#                   libraries
#

include ../MakeVars


lib: libtrkbbb$(TRKBBB_VERSION).so

libtrkbbb$(TRKBBB_VERSION).so : $(wildcard ./objs/*.o)
	$(CXX) $(LIBFLAGS) -o $@ $(wildcard ./objs/*.o)

clean: 
	$(RM) libtrkbbb$(TRKBBB_VERSION).so
	$(RM) ./objs/*.o

