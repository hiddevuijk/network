
#include "network.h"
#include "generate_network.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    
    int Nx = 4;
    int Ny = 4;
    int Nv = Nx*Ny;
    int Lx = 4.;

    Network net = generateNetwork(Nx,Ny,Lx);
    //Network net(Nv);

    //net.addEdge(0,1);
    //net.addEdge(1,2);
    //net.addEdge(2,3);
    //net.addEdge(3,0);

    //net.addEdge(4,5);
    //net.addEdge(5,6);
    //net.addEdge(6,7);
    //net.addEdge(7,0);

    //net.addEdge(0,4);
    //net.addEdge(1,4);
    //net.addEdge(1,5);
    //net.addEdge(2,5);
    //net.addEdge(2,6);
    //net.addEdge(3,6);
    //net.addEdge(3,7);

    //net.addEdge(1,8);    
    //net.addEdge(1,9);    
    //net.addEdge(2,9);    
    //net.addEdge(2,10);    
    //net.addEdge(3,10);    
    //net.addEdge(3,11);    

    net.polymerize(0,1);
    net.polymerize(1,2);

    net.deleteEdge(2,1);

    cout << "Network done ..." << endl;
    net.showPolymers();

    ofstream out("topology.txt");
    net.write(out);
    out.close();

    return 0;
}

