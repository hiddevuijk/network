

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
    g.vertices[0].bends[0].nextBend = &g.vertices[4].bends[0];
    g.vertices[4].bends[0].prevBend = &g.vertices[0].bends[0];
    
    cout << g.vertices[0].bends[0].nextBend->I << endl;
    cout << g.vertices[4].bends[0].prevBend->I << endl;
    cout << endl;
       
 
    //g.exchangeVertices(1,2);
    //g.exchangeVertices(0,3);
    //g.exchangeVertices(2,4);


    cout << g.vertices[0].bends[0].nextBend->I << endl;
    cout << g.vertices[4].bends[0].prevBend->I << endl;
    cout << endl;
       

    return 0;
}
