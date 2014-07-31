OPT=-g -O0

all: fsm.o state.o
	g++ $(OPT) main.cpp fsm.o state.o -o fsm

state.o: fsm.h state.cpp
	g++ -c $(OPT) state.cpp

fsm.o: fsm.h fsm.cpp
	g++ -c $(OPT) fsm.cpp

new: clean all

clean:
	rm *.o
