

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
    int Nx = 400;
    int Ny = 400;
    int Lx = 4;

    Graph g = generateNetwork(Nx,Ny,Lx);

    g.showBends();

    return 0;
}
