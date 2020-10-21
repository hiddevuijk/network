

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
    filaments.push_back(v);

    vector<int>::iterator it1 = find( filaments[0].begin(), filaments[0].end(), 5);
    vector<int>::iterator it2 = find( filaments[0].begin(), filaments[0].end(), 6);
    if( it1+1 == filaments[0].end() or it2+1 == filaments[0].end() ) {
        if( it1<it2) filaments[0].erase(it1,it2+1); 
        else         filaments[0].erase(it2, it1+1); 
    } else if( it1 == filaments[0].begin() or it2 == filaments[0].begin() ) {
        if( it1<it2) filaments[0].erase( it1, it2+1); 
        else         filaments[0].erase( it2, it1+1); 
    } else if( it1 > it2) {
        filaments.push_back( vector<int>() );        
        copy(it1, filaments[0].end(), back_inserter(filaments[filaments.size() - 1 ] ) );
        filaments[0].erase(it1, filaments[0].end() );
    } else {
        filaments.push_back( vector<int>() );        
        copy(it2, filaments[0].end(), back_inserter(filaments[filaments.size() - 1 ] ) );
        filaments[0].erase(it2, filaments[0].end() );
    }

    for(int i=0;i<filaments.size(); ++i) {
        cout << "filament " << i << endl;
        for(int j=0; j<filaments[i].size() ; ++j ) {
            cout << filaments[i][j] << ' ';
        }
        cout << endl << endl;
    } 


    return 0;
    ifstream in("topology.txt");

    Graph g(in);


    g.showAdj(); 
    return 0;
}
