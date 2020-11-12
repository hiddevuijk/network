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
    void showBends() const;
      
    int V() const { return Nv; }

    void set_position(int vi, double x, double y)
    { vertices[vi].x = x; vertices[vi].y = y; }

    void addEdge(int i, int j)
    {
        vertices[i].addAdj(j);
        vertices[j].addAdj(i);
    }

    void deleteEdge(int i, int j);
    void exchangeVertices(int i,int j);
    void addVertex() {
        vertices.push_back( Vertex(Nv) );
        ++Nv;
    }


    void deleteVertex(int i);
    int Nneighbours (int i) const { return vertices[i].Nneighbours(); } 
  //private:


    class Bend {
        // bend triplet a-b-c
      public:
        Bend(): a(-1), b(-1), c(-1), fIndex(-1) {}

        Bend(int a, int b, int c, int fIndex)
            : a(a), b(b), c(c), fIndex(fIndex)
            {}

        int a,b,c;
        double theta0;
        int fIndex;
    };

    void addBend(int vi, int viPrev, int viNext, int fIndex);

    class Vertex {
      public:
        Vertex(): index(-1) {} 
        Vertex(int index): index(index) {} 

        int index;
        double x,y;

        void set_index(int i) { index = i; }
        void addAdj(int vi);
        int Nneighbours() const { return adj.size(); }

        std::vector<int> adj;
        std::vector<Bend> bends;
    };
  
    bool isBendNext(int b, int c); // true if bend b-c-* exists
    bool isBendPrev(int b, int c); // true if bend *-b*c exists
    bool hasNext( Graph::Bend bend); // true if bend.c has bend bend.b-bend.c-*
    bool hasPrev( Graph::Bend bend); // ture if bend.a has bend *-bend.a - bend.b
    Graph::Bend nextBend( Graph::Bend bend); // return next bend
    Graph::Bend prevBend( Graph::Bend bend); // return previous bend

    int Nv; // number ov vertices
    std::vector<Vertex> vertices;
};

// check if vertex b has a bend with a->b->*
bool Graph::isBendNext(int a, int b)
{

    // loop over all bend with b in the middle
    std::vector<Graph::Bend>::iterator it = vertices[b].bends.begin();
    while( it != vertices[b].bends.end() ) {
        if( it->a == a) return true;
        ++it;
    }

    return false;
}

// check if vertex  b has a bend with *-b-c
bool Graph::isBendPrev(int b, int c)
{

    // loop over all bend with b in the middle
    std::vector<Graph::Bend>::iterator it = vertices[b].bends.begin();
    while( it != vertices[b].bends.end() ) {
        if( it->c == c) return true;
        ++it;
    }

    return false;
}


Graph::Bend Graph::nextBend( Graph::Bend bend)
{
    std::vector<Graph::Bend>::iterator it = vertices[bend.c].bends.begin();
    while( it != vertices[bend.c].bends.end() ) {
        if( it->fIndex == bend.fIndex ) return *it;
        ++it;
    }

    return Graph::Bend();
}

Graph::Bend Graph::prevBend( Graph::Bend bend)
{
    std::vector<Graph::Bend>::iterator it = vertices[bend.a].bends.begin();
    while( it != vertices[bend.a].bends.end() ) {
        if( it->fIndex == bend.fIndex ) return *it;
        ++it;
    }

    return Graph::Bend();
}

bool Graph::hasNext( Graph::Bend bend)
{ return isBendNext(bend.b, bend.c); }

bool Graph::hasPrev( Graph::Bend bend)
{ return isBendPrev(bend.a, bend.b); }

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

    // remove bends with this edge 


    // // delete bends involving this vertex
    //std::vector<Bend>::iterator it; 
    //// ni is the index iof a neighbour of vertex i
    //// vni is the vertex index of ni
    //int vni;
    //for( std::vector<int>::size_type  ni = 0;  ni < vertices[i].adj.size(); ++ni ){
    //    vni = vertices[i].adj[ni]; 
    //    // it iterates over the bends of vertex 
    //    it = vertices[vni].bends.begin();
    //    while( it != vertices[vni].bends.end() ) {
    //        if( it->a == i or it->c == i ){
    //            vertices[vni].bends.erase(it);
    //            break;
    //        }
    //        ++it;
    //    }
    //}
}

void Graph::exchangeVertices(int i, int j)
{
    //// change the index of the last vertex to i
    //vertices[i] = vertices[viLast];
    //vertices[i].index = i;

    //// change connection to vertex viLast to vertex i
    //// ai is the index in the adj list of vertex i
    //for(std::vector<int>::size_type ai=0; ai<vertices[i].adj.size(); ++ai ) {
    //    // vi_ai is the vertex to which vertex i is connected
    //    // change the edge vi_ai <-> viLast to vi_ai <-> i
    //    int vi_ai = vertices[i].adj[ai];
    //    std::vector<int>::iterator it = std::find( vertices[vi_ai].adj.begin(), vertices[vi_ai].adj.end(), viLast);
    //    *it = i;
    //}

    //// bends
}


void Graph::deleteVertex(int i)
{
   
    // delete all edge connected to vertex i
    while( vertices[i].adj.size() > 0 ) {
        deleteEdge(i, vertices[i].adj[0] );
    }

    int viLast = vertices.size() - 1; // index of the last vertex

    // exchange the indeces of vertex i and the last vertex
    exchangeVertices(i, viLast);   
    
    // remove last vertex
    //vertices.erase( viLast );
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

void Graph::showBends() const
{
    
    for(int iv=0; iv < Nv; ++iv) {
        std::cout << iv << " :\n";
        for( std::vector<int>::size_type ib=0; ib < vertices[iv].bends.size(); ++ib ) {
            std::cout << "\t " << vertices[iv].bends[ib].fIndex << '\t';
            std::cout << "\t " << vertices[iv].bends[ib].a << '\t';
            std::cout << "\t " << vertices[iv].bends[ib].c << '\n';
        }
        std::cout << std::endl;
    }
}

void Graph::addBend(int vi, int viPrev, int viNext, int fIndex) 
{ vertices[vi].bends.push_back( Graph::Bend(viPrev, vi, viNext, fIndex) ); }

#endif
