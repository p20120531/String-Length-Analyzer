CC = g++
CFLAGS = -g3
all: main.o mgrParse.o mgrDG.o pt.o ptnode.o dg.o
	$(CC) $(CFLAGS) main.o mgrParse.o mgrDG.o pt.o ptnode.o dg.o -o smt2dg
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -c
mgrParse.o: mgrParse.cpp
	$(CC) $(CFLAGS) mgrParse.cpp -c
mgrDG.o: mgrDG.cpp
	$(CC) $(CFLAGS) mgrDG.cpp -c
pt.o: pt.cpp
	$(CC) $(CFLAGS) pt.cpp -c
ptnode.o: ptnode.cpp
	$(CC) $(CFLAGS) ptnode.cpp -c
dg.o: dg.cpp
	$(CC) $(CFLAGS) dg.cpp -c
clean:
	rm *.o smt2dg
