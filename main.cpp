
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

    net.addBend(0,4,1);
    net.addBend(1,0,2);
    net.addBend(2,1,3);
    net.addBend(3,2,4);
    net.addBend(4,3,0);

    net.polymerize(0,1);
    net.polymerize(1,2);
    net.polymerize(2,3);
    net.polymerize(3,4);
    net.polymerize(4,0);

    net.depolymerize(1,2);

    cout << "showing polymer " << endl;
    net.showPolymer(0,1);
    cout << endl;
    net.showPolymer(3,4);

    cout << endl;


    return 0;
}

