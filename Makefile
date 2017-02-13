ifeq ($(mode),debug) 
	CPPFLAGS= $(DEF_CPPFLAG) -g -DDEBUG -D_DEBUG 
else
	CPPFLAGS= $(DEF_CPPFLAG) -O3 -DNDEBUG
	mode=release
endif

ObjDir = obj/$(mode)
IncDir= inc
SrcDir = src
IncDirs = -I$(IncDir)
CPPS=LibGastonForSofia.cpp database.cpp patterntree.cpp closeleg.cpp patterngraph.cpp graphstate.cpp legoccurrence.cpp path.cpp

DEF_CPPFLAG= -fPIC $(IncDirs)
BINDIR=bin/$(mode)

GCC=g++ $(CPPFLAGS) 

OBJS=$(patsubst %.cpp, $(ObjDir)/%.o, $(CPPS))

default: lib
all: lib test

lib: prepare $(OBJS)
	$(GCC) -shared -o $(BINDIR)/libGaston.so $(OBJS) 
test: prepare $(OBJS)
	$(GCC)  -o $(BINDIR)/test $(OBJS) 
clean:
	rm -r obj/*; rm -r bin/*

prepare:
	test -d $(ObjDir) || mkdir -p $(ObjDir)
	test -d $(BINDIR) || mkdir -p $(BINDIR)
$(ObjDir)/database.o: $(SrcDir)/database.cpp $(SrcDir)/database.h
	$(GCC) -c $(SrcDir)/database.cpp -o $(ObjDir)/database.o
database.h: $(SrcDir)/legoccurrence.h $(SrcDir)/misc.h
$(ObjDir)/patterntree.o: $(SrcDir)/patterntree.cpp $(SrcDir)/patterntree.h $(SrcDir)/patterngraph.h $(SrcDir)/graphstate.h
	$(GCC) -c $(SrcDir)/patterntree.cpp -o $(ObjDir)/patterntree.o
patterntree.h: $(SrcDir)/misc.h $(SrcDir)/database.h $(SrcDir)/legoccurrence.h $(SrcDir)/path.h $(SrcDir)/closeleg.h
$(ObjDir)/patterngraph.o: $(SrcDir)/patterngraph.cpp $(SrcDir)/patterngraph.h $(SrcDir)/graphstate.h
	$(GCC) -c $(SrcDir)/patterngraph.cpp -o $(ObjDir)/patterngraph.o
patterngraph.h: $(SrcDir)/closeleg.h
$(ObjDir)/LibGastonForSofia.o: $(SrcDir)/misc.h $(SrcDir)/database.h $(SrcDir)/LibGastonForSofia.cpp $(SrcDir)/path.h $(SrcDir)/graphstate.h $(IncDir)/LibGastonForSofia.h
	$(GCC) -c $(SrcDir)/LibGastonForSofia.cpp -o $(ObjDir)/LibGastonForSofia.o
$(ObjDir)/legoccurrence.o: $(SrcDir)/legoccurrence.h $(SrcDir)/legoccurrence.cpp $(SrcDir)/closeleg.h $(SrcDir)/database.h $(SrcDir)/graphstate.h
	$(GCC) -c $(SrcDir)/legoccurrence.cpp -o $(ObjDir)/legoccurrence.o
legoccurrence.h: $(SrcDir)/misc.h
$(ObjDir)/closeleg.o: $(SrcDir)/closeleg.cpp $(SrcDir)/closeleg.h $(SrcDir)/misc.h
	$(GCC) -c $(SrcDir)/closeleg.cpp -o $(ObjDir)/closeleg.o
$(SrcDir)/closeleg.h: $(SrcDir)/misc.h $(SrcDir)/legoccurrence.h
$(ObjDir)/path.o: $(SrcDir)/path.cpp $(SrcDir)/path.h $(SrcDir)/patterntree.h $(SrcDir)/patterngraph.h $(SrcDir)/graphstate.h
	$(GCC) -c $(SrcDir)/path.cpp -o $(ObjDir)/path.o
$(SrcDir)/path.h: $(SrcDir)/misc.h $(SrcDir)/database.h $(SrcDir)/legoccurrence.h $(SrcDir)/closeleg.h
$(ObjDir)/graphstate.o: $(SrcDir)/graphstate.cpp $(SrcDir)/graphstate.h $(SrcDir)/database.h $(SrcDir)/misc.h
	$(GCC) -c $(SrcDir)/graphstate.cpp -o $(ObjDir)/graphstate.o
$(SrcDir)/graphstate.h: $(SrcDir)/misc.h 
