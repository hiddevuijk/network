
#include "network.h"
#include "generate_network.h"

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

    Network net = generateNetwork(Nx,Ny,Lx,z);
    

    cout << "Network done ..." << endl;
    cout << net.averageConnectivity() << endl;
    ofstream out("topology.txt");
    net.write(out);
    out.close();

    return 0;
}

