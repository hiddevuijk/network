
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
    
    int Nx = 20;
    int Ny = Nx;
    double Lx = Nx;
    double Ly = Lx*sqrt(3/4.);
    double z = 3.4;

    Graph graph = generateGraph(Nx,Ny,Lx,z);
    
    Network network(graph);
    network.set_k(1.);
    network.set_Lx(Lx);
    network.set_Ly(Ly);
    cout << network.totalEnergy() << endl;;
    network.shear(2.);
    cout << network.totalEnergy() << endl;;

    ofstream top("topology.txt");
    graph.write(top);
    top.close();

    ofstream out("r.dat");
    network.savePositions(out);
    out.close();


    return 0;

}

