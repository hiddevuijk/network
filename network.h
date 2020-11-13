#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H

#include <vector>
#include <iostream>

class Network {
  public:

    class Vertex;
    class Bend;


    Network();
    Network(int Nv);
    ~Network();

    void addEdge(int i, int j);
    void addEdge(Vertex *vi, Vertex *vj);

    void deleteEdge(int i, int j);
    void deleteEdge(Vertex *vi, Vertex *vj);

    void addBend(int mid, int prev, int next);
    void addBend(Vertex *mid, Vertex *prev, Vertex *next);

    void polymerize(int i, int j);
    void polymerize(Vertex *vi, Vertex *vj);
    void polymerize(Bend *bi, Bend *bj);

    void showAdj();
    void showBends();

    class Vertex {
      public:
        Vertex(): index(-1) {}
        Vertex(int i) : index(i) {}
        int index;
        std::vector<Vertex*> adj;
        std::vector<Bend*> bends;
    };


    class Bend {
      public:
        Bend()
            : mid(nullptr), prev(nullptr), next(nullptr),
              prevBend(nullptr), nextBend(nullptr),
              filament(-1) {}

        Bend(Vertex *mid, Vertex *prev, Vertex *next)
            : mid(mid), prev(prev), next(next),
              prevBend(nullptr), nextBend(nullptr),
              filament(-1) {}
        Vertex *mid;
        Vertex *prev;
        Vertex *next;
        Bend* prevBend;
        Bend* nextBend;
        int filament;
        double theta0;
    };

    std::vector<Vertex*> vertices;


};


Network::Network() {};

Network::Network( int Nv)
: vertices( std::vector<Vertex*>(Nv, nullptr) )
{
    for( int vi=0; vi<Nv; ++vi) {
        vertices[vi] = new Vertex(vi);
    }
}


Network::~Network()
{ 
    for( std::vector<Vertex*>::size_type vi=0; vi < vertices.size(); ++vi) {
        delete vertices[vi];
        vertices[vi] = nullptr;
    }
}

void Network::addEdge(int i, int j)
{ addEdge(vertices[i], vertices[j] ); }

void Network::addEdge(Vertex *vi, Vertex *vj)
{
    // check if already exists??
    vi->adj.push_back( vj ); 
    vj->adj.push_back( vi ); 

}

void Network::deleteEdge(int i, int j)
{ deleteEdge( vertices[i], vertices[j]); }

void Network::deleteEdge( Vertex *vi, Vertex *vj)
{
    //in adj of vi, find vj
    std::vector<Vertex*>::iterator it = vi->adj.begin();
    while( it != vi->adj.end() ) {
        if( *it == vj ){
            vi->adj.erase(it);
            break;
        }
        ++it;
    }

    // in adj of vj, find vi
    it = vj->adj.begin();
    while( it != vi->adj.end() ) {
        if( *it == vi ) {
            vj->adj.erase(it);
            break;
        }
        ++it;
    }
}

void Network::addBend(int mid, int prev, int next)
{ addBend( vertices[mid], vertices[prev], vertices[next] ); }

void Network::addBend(Vertex *mid, Vertex *prev, Vertex *next)
{ mid->bends.push_back( new Bend(mid,prev,next) ); }

void Network::polymerize( int i, int j)
{ polymerize(vertices[i], vertices[j] ); }

void Network::polymerize( Vertex *vi, Vertex *vj )
{

    //find the bend with ? - vi - vj
    std::vector<Bend*>::iterator it_bi = vi->bends.begin(); 
    while( it_bi != vi->bends.end() ) {
        if( (*it_bi)->next == vj ) break;
        ++it_bi;
    }
    //if( it_bi == vi->bends.end() ) return;

    //find the bend with  vi - vj - ?
    std::vector<Bend*>::iterator it_bj = vj->bends.begin(); 
    while( it_bj != vj->bends.end() ) {
        if( (*it_bj)->prev == vi ) break;
        ++it_bj;
    }
    //if( it_bj == vj->bends.end() ) return;

    // polymerize bends
    polymerize( *it_bi, *it_bj);
    
}

void Network::polymerize( Bend *bi, Bend *bj)
{
    bi->nextBend = bj;
    bj->prevBend = bi;
}

void Network::showAdj()
{
    std::vector<Vertex*>::iterator it_v = vertices.begin();
    std::vector<Vertex*>::iterator it_adj;
    while( it_v != vertices.end() ) {
        std::cout << (*it_v)->index << ":";
        it_adj = (*it_v)->adj.begin();
        while( it_adj != (*it_v)->adj.end() ){
            std::cout << '\t' << (*it_adj)->index;
            ++it_adj;
        }
        std::cout << '\n';
        ++it_v;
    }
}

void Network::showBends()
{

    std::vector<Vertex*>::iterator it_v = vertices.begin();
    std::vector<Bend*>::iterator it_b;
    while( it_v != vertices.end() ) {
        std::cout << (*it_v)->index << ":\n";
        it_b = (*it_v)->bends.begin();
        while( it_b != (*it_v)->bends.end() ) {
            std::cout << '\t' << (*it_b)->prev->index;
            std::cout << '\t' << (*it_b)->mid->index;
            std::cout << '\t' << (*it_b)->next->index;
            std::cout << '\n';
            ++it_b;
        }
        ++it_v;
    }

}


#endif
