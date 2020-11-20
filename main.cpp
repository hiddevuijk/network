
#include "network.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    int Nv = 6;
    Network net(Nv);

    net.addEdge(0,1);
    net.addEdge(1,2);
    net.addEdge(2,3);
    net.addEdge(3,0);

    net.addBend(1,0,2);
    net.addBend(2,1,3);
    net.addBend(3,2,0);
    net.addBend(0,3,1);
    
    net.polymerize(0,1);
    net.polymerize(1,2);
    net.polymerize(2,3);
    net.polymerize(3,0);

    net.addEdge(2,4);
    net.addEdge(4,5);
    
    net.addBend(4,2,5);
    

    net.showAdj(); 
    cout << endl;
    cout << endl;
    net.showBends();
    cout << endl;
    cout << endl;
    net.showPolymers(); 

    return 0;
}

