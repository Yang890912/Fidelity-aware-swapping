dependency = Network/Node/Node.o Network/Shape/Shape.o Network/Graph/Graph.o Algorithm/AlgorithmBase/AlgorithmBase.o config.o
algorithm = Algorithm/MyAlgo2/MyAlgo2.o Algorithm/MyAlgo1/MyAlgo1.o Algorithm/MyAlgo3/MyAlgo3.o Algorithm/MyAlgo4/MyAlgo4.o Algorithm/MyAlgo5/MyAlgo5.o
algorithm2 = Algorithm/MyAlgo3/MyAlgo3.o Algorithm/MyAlgo1_time/MyAlgo1_time.o
gcc_flag = -g -Wall -Wextra -Os -Ofast --std=c++17

all: a.out

runtime: runtime.cpp $(dependency) $(algorithm2)
	g++ -fopenmp $(gcc_flag) runtime.cpp $(dependency) $(algorithm2) -o runtime

a.out: main.cpp $(dependency) $(algorithm)
	g++ -fopenmp $(gcc_flag) main.cpp $(dependency) $(algorithm)

cdf: cdf.cpp $(dependency) $(algorithm)
	g++ -fopenmp $(gcc_flag) cdf.cpp $(dependency) $(algorithm) -o cdf

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

Algorithm/MyAlgo1/MyAlgo1.o: Algorithm/MyAlgo1/MyAlgo1.h Algorithm/MyAlgo1/MyAlgo1.cpp config.h
	g++ -c $(gcc_flag) Algorithm/MyAlgo1/MyAlgo1.h Algorithm/MyAlgo1/MyAlgo1.cpp config.h
	mv MyAlgo1.o Algorithm/MyAlgo1/MyAlgo1.o

Algorithm/MyAlgo1_time/MyAlgo1_time.o: Algorithm/MyAlgo1_time/MyAlgo1_time.h Algorithm/MyAlgo1_time/MyAlgo1_time.cpp config.h
	g++ -c $(gcc_flag) Algorithm/MyAlgo1_time/MyAlgo1_time.h Algorithm/MyAlgo1_time/MyAlgo1_time.cpp config.h
	mv MyAlgo1_time.o Algorithm/MyAlgo1_time/MyAlgo1_time.o

Algorithm/MyAlgo2/MyAlgo2.o: Algorithm/MyAlgo2/MyAlgo2.h Algorithm/MyAlgo2/MyAlgo2.cpp config.h
	g++ -c $(gcc_flag) Algorithm/MyAlgo2/MyAlgo2.h Algorithm/MyAlgo2/MyAlgo2.cpp config.h
	mv MyAlgo2.o Algorithm/MyAlgo2/MyAlgo2.o

Algorithm/MyAlgo3/MyAlgo3.o: Algorithm/MyAlgo3/MyAlgo3.h Algorithm/MyAlgo3/MyAlgo3.cpp config.h
	g++ -c $(gcc_flag) Algorithm/MyAlgo3/MyAlgo3.h Algorithm/MyAlgo3/MyAlgo3.cpp config.h
	mv MyAlgo3.o Algorithm/MyAlgo3/MyAlgo3.o

Algorithm/MyAlgo4/MyAlgo4.o: Algorithm/MyAlgo4/MyAlgo4.h Algorithm/MyAlgo4/MyAlgo4.cpp config.h
	g++ -c $(gcc_flag) Algorithm/MyAlgo4/MyAlgo4.h Algorithm/MyAlgo4/MyAlgo4.cpp config.h
	mv MyAlgo4.o Algorithm/MyAlgo4/MyAlgo4.o

Algorithm/MyAlgo5/MyAlgo5.o: Algorithm/MyAlgo5/MyAlgo5.h Algorithm/MyAlgo5/MyAlgo5.cpp config.h
	g++ -c $(gcc_flag) Algorithm/MyAlgo5/MyAlgo5.h Algorithm/MyAlgo5/MyAlgo5.cpp config.h
	mv MyAlgo5.o Algorithm/MyAlgo5/MyAlgo5.o


config.o:	config.h config.cpp
	g++ -c $(gcc_flag) config.h config.cpp

clean:
	rm -f Network/Node/Node.o Network/Node/Node.h.gch
	rm -f Network/Graph/Graph.o Network/Graph/Graph.h.gch
	rm -f Algorithm/MyAlgo2/MyAlgo2.o Algorithm/MyAlgo2/MyAlgo2.h.gch
	rm -f config.o config.h.gch
	rm -f a.out
	find . -type f -name '*.o' -delete
	find . -type f -name '*.h.gch' -delete
	rm -f ../data/ans/*.ans
	rm -f ../data/log/*
	rm -f ../data/input/*
	rm cdf
	rm runtime