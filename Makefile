GCC=g++ -O3
ObjDir = obj
SrcDir = src
CPPS=main.cpp database.cpp patterntree.cpp closeleg.cpp patterngraph.cpp graphstate.cpp legoccurrence.cpp path.cpp
OBJS=$(patsubst %.cpp, $(ObjDir)/%.o, $(CPPS))
gaston: prepare $(OBJS)
	$(GCC) -o bin/Release/gaston $(OBJS) 
clean:
	rm $(OBJS); rm bin/Release/gaston

prepare:
	test -d $(ObjDir) || mkdir $(ObjDir)
	test -d bin || mkdir bin
	test -d bin/Release || mkdir bin/Release
$(ObjDir)/database.o: $(SrcDir)/database.cpp $(SrcDir)/database.h
	$(GCC) -c $(SrcDir)/database.cpp -o $(ObjDir)/database.o
database.h: $(SrcDir)/legoccurrence.h $(SrcDir)/misc.h
$(ObjDir)/patterntree.o: $(SrcDir)/patterntree.cpp $(SrcDir)/patterntree.h $(SrcDir)/patterngraph.h $(SrcDir)/graphstate.h
	$(GCC) -c $(SrcDir)/patterntree.cpp -o $(ObjDir)/patterntree.o
patterntree.h: $(SrcDir)/misc.h $(SrcDir)/database.h $(SrcDir)/legoccurrence.h $(SrcDir)/path.h $(SrcDir)/closeleg.h
$(ObjDir)/patterngraph.o: $(SrcDir)/patterngraph.cpp $(SrcDir)/patterngraph.h $(SrcDir)/graphstate.h
	$(GCC) -c $(SrcDir)/patterngraph.cpp -o $(ObjDir)/patterngraph.o
patterngraph.h: $(SrcDir)/closeleg.h
$(ObjDir)/main.o: $(SrcDir)/misc.h $(SrcDir)/database.h $(SrcDir)/main.cpp $(SrcDir)/path.h $(SrcDir)/graphstate.h
	$(GCC) -c $(SrcDir)/main.cpp -o $(ObjDir)/main.o
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
