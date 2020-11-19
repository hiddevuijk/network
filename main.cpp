
#include "network.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    int Nv = 8;
    Network net(Nv);

    net.addEdge(0,1); net.setVertexPosition(0, 0,0);
    net.addEdge(1,2); net.setVertexPosition(1, 1,1);
    net.addEdge(2,3); net.setVertexPosition(2, 2,0);
    net.addEdge(3,4); net.setVertexPosition(3, 3,0);
    net.addEdge(4,0);
    net.addEdge(4,1);
    net.addEdge(2,5);
    net.addEdge(2,6);
    net.addEdge(6,7);
    net.addEdge(5,7);

    net.addBend(0,4,1);
    net.addBend(1,0,2);
    net.addBend(2,1,3);
    net.polymerize(0,1);
    net.polymerize(1,2);


    net.addBend(2,5,6);
    net.addBend(6,2,7);
    net.addBend(7,6,5);
    net.addBend(5,7,2);
    net.polymerize(2,6);
    net.polymerize(6,7);
    net.polymerize(7,5);
    net.polymerize(5,2);
    

    net.showAdj(); 
    cout << endl;
    cout << endl;
    net.showBends();


    //ofstream out("topology.txt");
    //net.write(out);
    //out.close();

    cout << endl;
    cout << endl;
    cout << endl;

    net.showPolymers(); 
    return 0;
}

