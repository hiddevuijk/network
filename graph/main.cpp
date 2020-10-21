

#include "graph.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

int main()
{


    ifstream in("topology.txt");

    Graph g(in);


    g.showAdj(); 
    return 0;
}
