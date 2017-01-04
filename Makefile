all: main.o parse.o dg.o
	g++ main.o parse.o dg.o -o dg
main.o: main.cpp
	g++ main.cpp -c
parse.o: parse.cpp
	g++ parse.cpp -c
dg.o: dg.cpp
	g++ dg.cpp -c
clean:
	rm *.o dg
