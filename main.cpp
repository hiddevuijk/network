
#include "network.h"
#include "generate_network.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    
    int Nx = 6;
    int Ny = 6;
    int Lx = 6;
    double z = 3;

    Network net = generateNetwork(Nx,Ny,Lx,z);
    

    cout << "Network done ..." << endl;
    cout << net.averageConnectivity() << endl;
    ofstream out("topology.txt");
    net.write(out);
    out.close();

    return 0;
}

