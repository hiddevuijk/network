
test.exe: main.cpp network.h 
	g++ main.cpp -std=c++11 -Wall -lm -lgsl -lgslcblas -o test.exe -g -O0 -lpthread


