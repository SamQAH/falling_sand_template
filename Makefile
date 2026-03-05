
CC = g++
OP = -Wall
OT = -o out.exe

out.exe: control.o logic.o viewer.o tile.o
	$(CC) $(OP) *.o $(OT)

control.o: control.h control.cpp
	$(CC) $(OP) -c control.cpp

logic.o: logic.h logic.cpp
	$(CC) $(OP) -c logic.cpp

viewer.o: viewer.h viewer.cpp
	$(CC) $(OP) -c viewer.cpp

tile.o: tile.h tile.cpp
	$(CC) $(OP) -c tile.cpp

clean:
	rm *.o out.exe

test:
	make
	out.exe

.PHONY: clean test

