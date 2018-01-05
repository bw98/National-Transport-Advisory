CXXFLAGS = -Wall -g -std=c++11
test : ALGraph.o main.o
	g++ $(CXXFLAGS) ALGraph.o main.o -o test
main.o : 
	g++ $(CXXFLAGS) -c main.cpp
ALGraph.o : 
	g++ $(CXXFLAGS) -c ALGraph.cpp
clean : 
	rm ALGraph.o main.o test
