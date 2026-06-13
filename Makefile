
CC = g++ -std=c++11
OP = -Wall -MMD -O0 -pedantic-errors
FLAGS =  
OT = -o out.exe

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

out.exe: $(OBJS)
	$(CC) $(OP) $(OBJS) $(OT)

%.o: %.cpp
	$(CC) $(OP) -c $< $(FLAGS) -o $@ 

fast:
	make FLAGS="-DVEC_UNSAFE"

debug:
	make FLAGS="-DVERBOSE -DDEBUG"

clean:
	rm -f *.o *.d out.exe

test:
	make
	out.exe

.PHONY: clean test fast debug

