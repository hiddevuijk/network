
test.exe: main.cpp graph.h  vec2.h generate_graph.h network.h
	g++ main.cpp -std=c++11 -Wall -lm -lgsl -lgslcblas -o test.exe -g -O3 -lpthread


