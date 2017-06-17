CC = g++
CFLAGS = -g3
TAG = ctags
TAGFLAGS = -R
all: main.o kaluzaMgrRead.o kaluzaMgrDG.o pt.o ptnode.o dg.o dgnode.o utility.o autMgr.o aut.o vmtnode.o tgraph.o
	$(CC) $(CFLAGS) main.o kaluzaMgrRead.o kaluzaMgrDG.o pt.o ptnode.o dg.o dgnode.o utility.o autMgr.o aut.o vmtnode.o tgraph.o -o ../../bin/sla
	$(TAG) $(TAGFLAGS)
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -c
kaluzaMgrRead.o: kaluzaMgrRead.cpp
	$(CC) $(CFLAGS) kaluzaMgrRead.cpp -c
kaluzaMgrDG.o: kaluzaMgrDG.cpp
	$(CC) $(CFLAGS) kaluzaMgrDG.cpp -c
pt.o: pt.cpp
	$(CC) $(CFLAGS) pt.cpp -c
ptnode.o: ptnode.cpp
	$(CC) $(CFLAGS) ptnode.cpp -c
dg.o: dg.cpp
	$(CC) $(CFLAGS) dg.cpp -c
dgnode.o: dgnode.cpp
	$(CC) $(CFLAGS) dgnode.cpp -c
utility.o: utility.cpp
	$(CC) $(CFLAGS) utility.cpp -c
autMgr.o: autMgr.cpp
	$(CC) $(CFLAGS) autMgr.cpp -c
aut.o: aut.cpp
	$(CC) $(CFLAGS) aut.cpp -c
vmtnode.o: vmtnode.cpp
	$(CC) $(CFLAGS) vmtnode.cpp -c
tgraph.o: tgraph.cpp
	$(CC) $(CFLAGS) tgraph.cpp -c
clean:
	rm *.o tags ../../bin/sla
