
#include "graph.h"
#include "generate_graph.h"
#include "network.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    
    int Nx = 10;
    int Ny = Nx;
    int Lx = 10;
    double z = 3.4;

    Graph graph = generateGraph(Nx,Ny,Lx,z);
    vector<vector<int> > edges = graph.getEdges();
    Network network(graph);
    network.shear(0.9);

    network.savePositions(cout);


    return 0;

}

