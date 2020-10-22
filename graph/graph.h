#ifndef GUARD_GRAPH_H
#define GUARD_GRAPH_H

#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>



class Graph
{
  public:
    Graph(): Nv(0) {};
    Graph(int Nv); 
    Graph( std::ifstream& in);

    void showAdj() const;
      
    int V() const { return Nv; }
    void set_position(int vi, double x, double y)
    { vertices[vi].x = x; vertices[vi].y = y; }

    void addEdge(int i, int j)
    {
        vertices[i].addAdj(j);
        vertices[j].addAdj(i);
    }

    void deleteEdge(int i, int j);

    void addVertex() {
        vertices.push_back( Vertex(Nv) );
        ++Nv;
    }

    void deleteVertex(int i);

  private:

    class Bend {
      public:
        int a,b,c;
        double theta0;
        int next, prev;
        int fIndex;
    };

    class Vertex {
      public:
        Vertex(): index(-1) {} 
        Vertex(int index): index(index) {} 

        int index;
        double x,y;

        void set_index(int i) { index = i; }
        void addAdj(int vi);


        std::vector<int> adj;
        std::vector<Bend> bends;
    };
  
    int Nv;
    std::vector<Vertex> vertices;
    std::vector<Vertex*> filaments;
};


void Graph::Vertex::addAdj(int vi)
{
    // if vp not in adj, add vp to adj
    if( std::find(adj.begin(), adj.end(), vi) == adj.end() ) {
        adj.push_back( vi );
    }
}



Graph::Graph(int Nv)
: Nv(Nv), vertices(Nv)
{
    for(int i=0; i<Nv; ++i) {
        vertices[i].index = i;
    }
}

Graph::Graph( std::ifstream& in)
{
    in >> Nv;
    vertices = std::vector<Vertex>(Nv);
    for(int i=0; i<Nv; ++i) {
        vertices[i].index = i;
    }

    int v,w;
    while( ( in >> v ) and ( in >> w) ){
        addEdge(v,w);
    }

}


void Graph::deleteEdge(int i, int j)
{
    std::vector<int>::iterator it =  std::find( vertices[i].adj.begin(), vertices[i].adj.end(), j);

    // if edge exist, remove it from adj[i] and adj[j]
    if( it != vertices[i].adj.end() ) {
        vertices[i].adj.erase(it);
        it = std::find( vertices[j].adj.begin(), vertices[j].adj.end(), i);
        vertices[j].adj.erase(it);
    }

}

void Graph::deleteVertex(int i)
{
    // delete all edge connected to vertex i
    while( vertices[i].adj.size() > 0 ) {
        deleteEdge(i, vertices[i].adj[0] );
    }

    int viLast = vertices.size() - 1; // index of the last vertex
    // change the index of the last vertex to i
    vertices[i] = vertices[viLast];
    vertices[i].index = i;
    // change connection to vertex viLast to vertex i
    for(std::vector<int>::size_type ai=0; ai<vertices[i].adj.size(); ++ai ) {
        std::vector<int>::iterator it = std::find( vertices[ai].adj.begin(), vertices[ai].adj.end(), i);
        *it = i;
    }
    vertices.erase( vertices.end() - 1 );
    Nv -= 1;
}

void Graph::showAdj() const
{
    
    for(int iv=0; iv < Nv; ++iv) {
        std::cout << iv << " : ";
        for( std::vector<int>::size_type ie=0; ie < vertices[iv].adj.size(); ++ie ) {
            std::cout << vertices[iv].adj[ie] << " ";
        }
        std::cout << std::endl;

    }
}



#endif
