
CC = g++
OP = -Wall -MMD -O0
OT = -o out.exe

out.exe: control.o logic.o viewer.o tile.o parser.o
	$(CC) $(OP) *.o $(OT)

control.o: control.h control.cpp
	$(CC) $(OP) -c control.cpp

logic.o: logic.h logic.cpp
	$(CC) $(OP) -c logic.cpp

viewer.o: viewer.h viewer.cpp
	$(CC) $(OP) -c viewer.cpp

tile.o: tile.h tile.cpp
	$(CC) $(OP) -c tile.cpp

parser.o: parser.h parser.cpp
	$(CC) $(OP) -c parser.cpp
fast:
	$(CC) $(OP) -c logic.cpp -DVEC_UNSAFE
	make

clean:
	rm *.o *.d out.exe

test:
	make
	out.exe 2> error.txt

.PHONY: clean test fast

