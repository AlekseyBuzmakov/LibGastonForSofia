GCC=g++ -O3
ALLFILES=main.cpp database.cpp database.h misc.h patterntree.cpp patterntree.h closeleg.cpp closeleg.h patterngraph.cpp patterngraph.h graphstate.cpp graphstate.h  
OBJS=main.o database.o path.o legoccurrence.o patterntree.o patterngraph.o closeleg.o graphstate.o 
gaston: $(OBJS)
	$(GCC) -o gaston $(OBJS) 
clean:
	rm $(OBJS); rm gaston
database.o: database.cpp database.h
	$(GCC) -c database.cpp
database.h: legoccurrence.h misc.h
patterntree.o: patterntree.cpp patterntree.h patterngraph.h graphstate.h
	$(GCC) -c patterntree.cpp
patterntree.h: misc.h database.h legoccurrence.h path.h closeleg.h
patterngraph.o: patterngraph.cpp patterngraph.h graphstate.h
	$(GCC) -c patterngraph.cpp
patterngraph.h: closeleg.h
main.o: misc.h database.h main.cpp path.h graphstate.h
	$(GCC) -c main.cpp
legoccurrence.o: legoccurrence.h legoccurrence.cpp closeleg.h database.h graphstate.h
	$(GCC) -c legoccurrence.cpp
legoccurrence.h: misc.h
closeleg.o: closeleg.cpp closeleg.h misc.h
	$(GCC) -c closeleg.cpp
closeleg.h: misc.h legoccurrence.h
path.o: path.cpp path.h patterntree.h patterngraph.h graphstate.h
	$(GCC) -c path.cpp
path.h: misc.h database.h legoccurrence.h closeleg.h
graphstate.o: graphstate.cpp graphstate.h database.h misc.h
	$(GCC) -c graphstate.cpp
graphstate.h: misc.h 
