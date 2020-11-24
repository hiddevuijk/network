
#include "network.h"
#include "generate_network.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    
    int Nx = 5;
    int Ny = 5;
    int Lx = 4.;

    Network net = generateNetwork(Nx,Ny,Lx);
    //Network net(Nx);
    //net.addEdge(0,1);
    //net.addEdge(1,2);
    //net.addEdge(2,3);
    //net.addEdge(3,0);

    //net.addBend(1,0,2);
    //net.addBend(2,1,3);
    //net.addBend(3,2,0);
    //net.addBend(0,3,1);

    //net.polymerize(1,2);
    //net.polymerize(2,3);
    //net.polymerize(3,0);
    //net.polymerize(0,1);

    //net.deleteEdge(0,1);

    cout << "Network done ..." << endl;

    ofstream out("topology.txt");
    net.write(out);
    out.close();

    return 0;
}

