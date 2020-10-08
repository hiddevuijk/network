
test.exe: main.cpp network.h energy.h system.h
	g++ main.cpp -std=c++11 -Wall -lm -lgsl -lgslcblas -o test.exe -g -lpthread


