OPT=-g -O0

all: fsm.o state.o decision_tree.o
	g++ $(OPT) main.cpp fsm.o state.o decision_tree.o -o fsm

decision_tree.o: fsm.h decision_tree.h decision_tree.cpp
	g++ -c $(OPT) decision_tree.cpp

state.o: fsm.h state.cpp
	g++ -c $(OPT) state.cpp

fsm.o: fsm.h fsm.cpp
	g++ -c $(OPT) fsm.cpp

new: clean all

clean:
	rm *.o
