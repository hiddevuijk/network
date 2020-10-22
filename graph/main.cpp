

#include "graph.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

int main()
{
    vector<int> v = { 0, 1, 2, 3, 4, 5, 6};
    vector<vector<int> > filaments; 

    ifstream in("topology.txt");

    Graph g(in);


    g.showAdj(); 
    return 0;
}
