

#include "graph.h"
#include "paths.h"
#include "paths_bfs.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

int main()
{



    ifstream in("graph.txt");
    Graph g(in);
    //g.deleteEdge(4,5);
    g.showAdj();
    cout << endl;


    int v = 2;
    cout << "All nodes not connected to " << v << " are:\n";
    PathsBFS<Graph> paths(g,v);


    for( int vi=0; vi<g.V(); ++vi) {
        cout <<vi << " : " <<  paths.distance(vi) << endl;
        //if( !paths.hasPathTo(vi) ) cout << vi << endl;
    }
    return 0;
}
