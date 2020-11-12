

#include "graph.h"
#include "generate_network.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

int main()
{

    int Nx = 4;
    int Ny = 4;
    double Lx = 4;
    Graph g = generateNetwork(Nx, Ny, Lx);

       

    return 0;
}
