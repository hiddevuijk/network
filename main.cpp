
#include "network.h"
#include "generate_network.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    //int Nv = 7;
    //Network net(Nv);

    //net.addEdge(0,1);
    //net.addEdge(1,2);
    //net.addEdge(2,3);
    //net.addEdge(4,2);
    //net.addEdge(2,5);
    //net.addEdge(5,6);

    //net.addBend(1,0,2);
    //net.addBend(2,1,3);
    //net.addBend(2,4,5);
    //net.addBend(5,2,6);

    //net.polymerize(1,2);
    //net.polymerize(2,5);
    
    int Nx = 4;
    int Ny = 4;
    int Lx = 4.;

    Network net = generateNetwork(Nx,Ny,Lx);

    cout << "Network done ..." << endl;
    net.showPolymers();

    ofstream out("topology.txt");
    net.write(out);
    out.close();

    return 0;
}

