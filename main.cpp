
#include "network.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    int Nv = 5;
    Network net(Nv);

    net.addEdge(0,1);
    net.addEdge(1,2);
    net.addEdge(2,3);
    net.addEdge(3,4);
    net.addEdge(4,0);

    net.addBend(1,0,2);


    ofstream out("topology.txt");
    net.write(out);
    out.close();

    cout << endl;


    return 0;
}

