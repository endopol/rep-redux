all: fsm.o
	g++ main.cpp fsm.o -o fsm

fsm.o: fsm.h fsm.cpp
	g++ -c fsm.cpp

clean:
	rm *.o
