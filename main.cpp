
#include "network.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    int Nv = 16;
    Network net(Nv);

    net.addEdge(0,1);
    net.addEdge(0,4);
    net.addEdge(1,2);
    net.addEdge(1,4);
    net.addEdge(1,5);
    net.addEdge(2,5);
    net.addEdge(2,3);
    net.addEdge(4,5);

    net.addBend(1,0,2);
    net.addBend(2,1,3);

    net.polymerize(1,2);

    net.showAdj();
    cout << endl;

    vector<vector<int> > edges = net.getEdges(); 
    for(vector<vector<int> >::size_type i =0; i<edges.size(); ++i)  {
        cout << edges[i][0] << '\t' << edges[i][1] << '\n';
    }
    

    return 0;
}

