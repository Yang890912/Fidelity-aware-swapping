dependency = Network/Node/Node.o Network/Shape/Shape.o Network/Graph/Graph.o Algorithm/AlgorithmBase/AlgorithmBase.o config.o
gcc_flag = -g -Wall -Wextra -O3 --std=c++2a

all: a.out

a.out: main.cpp $(dependency) Algorithm/MyAlgo2/MyAlgo2.o
	g++ -fopenmp $(gcc_flag) main.cpp $(dependency) Algorithm/MyAlgo2/MyAlgo2.o

Network/Node/Node.o: Network/Node/Node.cpp Network/Node/Node.h config.h
	g++ -c $(gcc_flag) Network/Node/Node.cpp Network/Node/Node.h config.h
	mv Node.o Network/Node/Node.o

Network/Shape/Shape.o: Network/Shape/Shape.cpp Network/Shape/Shape.h config.h
	g++ -c $(gcc_flag) Network/Shape/Shape.cpp Network/Shape/Shape.h config.h
	mv Shape.o Network/Shape/Shape.o

Network/Graph/Graph.o: Network/Graph/Graph.cpp Network/Graph/Graph.h config.h
	g++ -c $(gcc_flag) Network/Graph/Graph.cpp Network/Graph/Graph.h config.h
	mv Graph.o Network/Graph/Graph.o

Algorithm/AlgorithmBase/AlgorithmBase.o: Algorithm/AlgorithmBase/AlgorithmBase.h Algorithm/AlgorithmBase/AlgorithmBase.cpp config.h
	g++ -c $(gcc_flag) Algorithm/AlgorithmBase/AlgorithmBase.h Algorithm/AlgorithmBase/AlgorithmBase.cpp config.h
	mv AlgorithmBase.o Algorithm/AlgorithmBase/AlgorithmBase.o

Algorithm/MyAlgo2/MyAlgo2.o: Algorithm/MyAlgo2/MyAlgo2.h Algorithm/MyAlgo2/MyAlgo2.cpp config.h
	g++ -c $(gcc_flag) Algorithm/MyAlgo2/MyAlgo2.h Algorithm/MyAlgo2/MyAlgo2.cpp config.h
	mv MyAlgo2.o Algorithm/MyAlgo2/MyAlgo2.o

config.o:	config.h config.cpp
	g++ -c $(gcc_flag) config.h config.cpp

clean:
	rm -f Network/Node/Node.o Network/Node/Node.h.gch
	rm -f Network/Graph/Graph.o Network/Graph/Graph.h.gch
	rm -f Algorithm/MyAlgo2/MyAlgo2.o Algorithm/MyAlgo2/MyAlgo2.h.gch
	rm -f config.o config.h.gch
	find . -type f -name '*.o' -delete
	find . -type f -name '*.h.gch' -delete
	rm -f ../data/ans/*
	rm -f ../data/log/*
	rm -f ../data/input/*