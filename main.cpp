
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
    int Ny = 10;
    int Lx = 6;
    double z = 3.4;

    Graph graph = generateGraph(Nx,Ny,Lx,z);
    

    cout << "Network done ..." << endl;
    cout << graph.averageConnectivity() << endl;
    ofstream out("topology.txt");
    graph.write(out);
    out.close();

    return 0;
}

