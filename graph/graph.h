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

    void addVertexToFilament(int fi, int i);

    // remove bond i, j from filament fi
    // create new filament with right side of old one
    void cutFilament(int fi, int i, int j);

  private:
    class Vertex {
      public:
        Vertex(): index(-1) {} 
        Vertex(int index): index(index) {} 

        int index;
        double x,y;

        void set_index(int i) { index = i; }
        void addAdj(int vi);


        std::vector<int> adj;
        std::vector<int> filaments;
    };
  
    int Nv;
    std::vector<Vertex> vertices;

    std::vector<std::vector<int> > filaments;
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


void Graph::addVertexToFilament(int fi, int i)
{
    filaments[fi].push_back(i); 
    vertices[i].filaments.push_back(fi);
}

void Graph::cutFilament(int fi, int i, int j)
{
    // can be faster
    std::vector<int>::iterator it_i = std::find( filaments[fi].begin(), filaments[fi].end() , i);
    std::vector<int>::iterator it_j = std::find( filaments[fi].begin(), filaments[fi].end() , j);

    int NnewFil = filaments.size();
    if( it_i+1 == filaments[fi].end() or it_j+1 == filaments[fi].end() ) {
        if(it_i<it_j) filaments[fi].erase( it_i, it_j+1);
        else        filaments[fi].erase( it_j, it_i+1);
        //  vertices i and j no longer belong to filaments fi
        it_i = std::find( vertices[i].filaments.begin(), vertices[i].filaments.end(), fi);
        vertices[i].filaments.erase(it_i);
        it_j = std::find( vertices[j].filaments.begin(), vertices[j].filaments.end(), fi);
        vertices[j].filaments.erase(it_j);
    }else if( it_i == filaments[fi].begin() or it_j == filaments[fi].begin() ) {
        if(it_i<it_j) filaments[fi].erase(it_i, it_j+1);
        else          filaments[fi].erase(it_j, it_i+1);
        //  vertices i and j no longer belong to filaments fi
        it_i = std::find( vertices[i].filaments.begin(), vertices[i].filaments.end(), fi);
        vertices[i].filaments.erase(it_i);
        it_j = std::find( vertices[j].filaments.begin(), vertices[j].filaments.end(), fi);
        vertices[j].filaments.erase(it_j);
    }else if( it_i > it_j ) {
        filaments.push_back( vector<int>() );
        std::copy(it_i, fialments[fi].end(), back_inserter(filaments[filaments.size() - 1]);

        it_i = filaments[NnewFil].begin();
        it_j = filaments[NnewFil].end();
        while( it_i < it_j ){
            for(std::vector<int> it = it_i->filaments.begin();
                it < filaments.end(); ++it ) {
                if( *it == fi ) {
                    *it = NnewFil;
                    break;
                }
        } 
    } else {
        filaments.push_back( vector<int>() );

    }
}



#endif
