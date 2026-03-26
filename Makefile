
CC = g++ -std=c++11
OP = -Wall -MMD -O0
OT = -o out.exe

out.exe: control.o logic.o viewer.o tile.o json.o
	$(CC) $(OP) *.o $(OT)

control.o: control.h control.cpp
	$(CC) $(OP) -c control.cpp

logic.o: logic.h logic.cpp
	$(CC) $(OP) -c logic.cpp

viewer.o: viewer.h viewer.cpp
	$(CC) $(OP) -c viewer.cpp

tile.o: tile.h tile.cpp
	$(CC) $(OP) -c tile.cpp

json.o: json.h json.cpp
	$(CC) $(OP) -c json.cpp

fast:
	$(CC) $(OP) -c logic.cpp -DVEC_UNSAFE
	make

clean:
	rm *.o *.d out.exe

test:
	make
	out.exe

.PHONY: clean test fast

