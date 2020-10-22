

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
    int Ny = 3;
    int Lx = 4;

    Graph g = generateNetwork(Nx,Ny,Lx);
    //g.showAdj();

    g.deleteVertex(6);
    cout << endl;
    g.showAdj();

    return 0;
}
