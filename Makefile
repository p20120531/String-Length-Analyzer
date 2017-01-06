all: main.o parse.o pt.o
	g++ main.o parse.o pt.o -o smt2dg
main.o: main.cpp
	g++ main.cpp -c
parse.o: parse.cpp
	g++ parse.cpp -c
dg.o: pt.cpp
	g++ pt.cpp -c
clean:
	rm *.o smt2dg
