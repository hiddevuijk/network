#ifndef GUARD_UNDIRECTED_GRAPH_H
#define GUARD_UNDIRECTED_GRAPH_H

#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

class Graph
{
  public:
    // create a graph with V vertices
    Graph(int V);
    // create a graph from input stream
    Graph( std::ifstream& in);

    // add edge between v and w
    void addEdge(int v, int w);
    void deleteEdge(int v, int w);
    void deleteVertex(int v);

    // return # vertices
    int V() const { return NV; }
    // return # edges
    int E() const { return NE; }

    void showAdj() const;

    const std::vector<int>::const_iterator get_iterator(int v)
            const { return adj[v].begin(); }
    int Nneighbours(int vi) const { return adj[vi].size(); }

    int neighbourIndex(int vi, int ni) const { return adj[vi][ni]; }
  private:
    int NV; 
    int NE;

    std::vector<std::vector<int> > adj;

};

void Graph::showAdj() const
{
    
    for(int iv=0; iv < NV; ++iv) {
        std::cout << iv << " : ";
        for(std::vector<int>::size_type ie=0;ie< adj[iv].size(); ++ie) {
            std::cout << adj[iv][ie] << " ";
        }
        std::cout << std::endl;

    }
}

void Graph::addEdge(int v, int w)
{
    NE += 1;
    adj[v].push_back(w);
    adj[w].push_back(v);

}

void Graph::deleteVertex(int v) 
{

}
void Graph::deleteEdge(int v, int w)
{
    std::vector<int>::iterator it = std::find(adj[v].begin(), adj[v].end(), w);
    if( it != adj[v].end() ) {
        adj[v].erase(it);
        it = std::find(adj[w].begin(), adj[w].end(), v);
        adj[w].erase(it);
    }

}

Graph::Graph( std::ifstream& in)
{
    NE = 0;
    in >> NV;
    adj = std::vector<std::vector<int> >(NV);
    int v,w;
    while( ( in >> v ) and ( in >> w) ){
        addEdge(v,w);
    }

}

#endif
