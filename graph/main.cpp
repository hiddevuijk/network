

#include "graph.h"
#include "generate_network.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

int main()
{

    int Nv = 6;
    Graph g;    
    for( int vi =0; vi < Nv; ++vi ) g.addVertex();

    g.addEdge(0,1);
    g.addEdge(0,4);
    g.addEdge(1,2);
    g.addEdge(1,4);
    g.addEdge(1,5);
    g.addEdge(2,4);
    g.addEdge(2,3);
    g.addEdge(3,5);

    g.addBend(0,1,4);
    g.addBend(4,0,2);

    g.polymerize(0,4);


    return 0;
}
