#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H


/*
    to do:
            -- throw error when polymerizing nodes without a bend
            -- throw other errors
*/

#include "vec2.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

class Network {
  public:

    class Vertex;
    class Edge;
    class Bend;


    Network();
    Network(int Nv);
    // Network ( infile) read network from file
    ~Network();

    void write( std::ofstream& out);
    int addVertex();
    int addVertex(double x, double y);

    void setVertexPosition(int i, double x, double y);

    void addEdge(int i, int j, int xb=0, int yb=0, double l0=0);
    void deleteEdge(int i, int j);

    void addBend(int mid, int va, int vb);
    void deleteBend(int mid, int va);
    void deleteAllBend(int vi, int vj); // delete all Bends with edge vi-vj

    void polymerize(int i, int j);
    void depolymerize(int i, int j); // test 
    void depolymerize(int i); // test // remove from all polymers

    // this changes the indices!!!
    //void removeUnconectedVertices(); // --
    // ....

    std::vector<std::vector<int> > getEdges() const;
    //std::vector<std::vector<int> > getBends() const; // ---
    std::vector<std::vector<int> > getPolymers();

    double averageConnectivity() const;

    void showAdj() const;
    void showBends() const;
    void showPolymers();
    // print polymer starting at i through j
    //void showPolymer(int i, int j) const;


    class Vertex {
      public:
        Vertex(): index(-1) {}
        Vertex(int i) : index(i) {}
        Vertex(int i, double x, double y) : index(i), r(x,y) {}
        ~Vertex();
        int index;
        std::vector<Edge*> edges;
        std::vector<Bend*> bends;

        // add location info
        vec2 r;
    };

    class Edge {
      public:

        Edge(): from(nullptr), to(nullptr) {}

        Edge( Vertex *from, Vertex *to, int xb=0, int yb=0, double l0=0)
            : from(from), to(to), xb(xb), yb(yb), l0(l0) {}

        Vertex *from, *to;
        int xb, yb;
        double l0; 
    };


    class Bend {
      public:
        Bend()
            : mid(nullptr), a(nullptr), b(nullptr),
              prevBend(nullptr), nextBend(nullptr),
              index(-1), filament(-1) {}

        Bend( Vertex *mid, Edge* a, Edge *b )
            : mid(mid), a(a), b(b),
              prevBend(nullptr), nextBend(nullptr),
              filament(-1) {}

        Bend( Vertex *mid, Edge* a, Edge *b, int index)
            : mid(mid), a(a), b(b),
              prevBend(nullptr), nextBend(nullptr),
              index(index), filament(-1) {}
        Vertex* previousVertex() const;
        Vertex* nextVertex() const;

        Vertex *mid;
        Edge *a, *b;
        Bend *prevBend, *nextBend;
        int index;
        int filament;
        double theta0;
    };

    private:
        std::vector<Vertex*> vertices;

        void deleteVertex(int i);
        void deleteVertex(Vertex *vi);
        void exchangeVertex(int i, int j);
        void exchangeVertex(Vertex *vi, Vertex *vj);

        void addEdge(Vertex *vi, Vertex *vj, int xb=0, int yb=0, double l0=0);
        void deleteEdge(Vertex *vi, Vertex *vj);
        void deleteEdge( std::vector<Edge*>::iterator it);

        void addBend(Vertex *mid, Vertex *prev, Vertex *next);
        void addBend(Vertex *mid, Edge *a, Edge *b);
        void deleteBend(Vertex *mid, Vertex *va);
        void deleteAllBend(Vertex *vi, Vertex *vj);
        void deleteBend(Bend *bend); 
        void deleteBend( std::vector<Bend*>::iterator it_bend);

        void polymerize(Vertex *vi, Vertex *vj);
        void polymerize(Bend *bi, Bend *bj); 

        void depolymerize( Vertex *vi, Vertex *vj); 
        void depolymerize( Vertex *vi);
        void depolymerize( Bend *bi, Bend *bj);
        void depolymerize( Bend *bi); 

        void resetFilamentIndex();
        Bend* firstBend( Bend *bend) const;
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

int Network::addVertex()
{
    std::vector<Vertex*>::size_type  Nv = vertices.size();
    vertices.push_back( new Vertex(Nv) );
    return Nv;
}

int Network::addVertex(double x, double y)
{
    std::vector<Vertex*>::size_type  Nv = vertices.size();
    vertices.push_back( new Vertex(Nv, x, y) );
    return Nv;
}

void Network::setVertexPosition(int i, double x, double y)
{
    vertices[i]->r.x = x;
    vertices[i]->r.y = y;
}

void Network::polymerize( Bend *bi, Bend *bj)
{
    bi->nextBend = bj;
    bj->prevBend = bi;
}

void Network::polymerize( Vertex *vi, Vertex *vj )
{

    //find the bend with ? - vi - vj
    std::vector<Bend*>::iterator it_bi = vi->bends.begin(); 
    while( it_bi != vi->bends.end() ) {
        if( (*it_bi)->a->to == vj or (*it_bi)->b->to == vj ) break;
        ++it_bi;
    }
    //if( it_bi == vi->bends.end() ) return;

    //find the bend with  vi - vj - ?
    std::vector<Bend*>::iterator it_bj = vj->bends.begin(); 
    while( it_bj != vj->bends.end() ) {
        if( (*it_bj)->a->from == vi or (*it_bi)->b->from == vi ) break;
        ++it_bj;
    }
    //if( it_bj == vj->bends.end() ) return;

    // polymerize bends
    polymerize( *it_bi, *it_bj);
    
}

void Network::polymerize( int i, int j)
{ polymerize(vertices[i], vertices[j] ); }

void Network::depolymerize( Bend *bi, Bend *bj)
{
    // if bi-bj is not on a polymer, don't do anything
    if( bi->nextBend != bj or bj->prevBend != bi) return;

    bi->nextBend = nullptr;
    bj->prevBend = nullptr; 
}

void Network::depolymerize( Bend *b)
{
    if( b->nextBend != nullptr ) depolymerize(b, b->nextBend);
    if( b->prevBend != nullptr ) depolymerize(b, b->prevBend);
}

void Network::depolymerize( Vertex *vi)
{
    std::vector<Bend*>::iterator it_b = vi->bends.begin();
    while( it_b != vi->bends.end() ) {
        depolymerize( *it_b);
        ++it_b;
    }
}

void Network::depolymerize(int i)
{ depolymerize(vertices[i]); }


void Network::depolymerize( Vertex *vi, Vertex *vj)
{

    std::vector<Bend*>::iterator it_b = vi->bends.begin();
    while( it_b != vi->bends.end() ){
        if( (*it_b)->nextBend->mid == vj ) {
            depolymerize( *it_b, (*it_b)->nextBend);
            break;
        }
        if( (*it_b)->prevBend->mid == vj ) {
            depolymerize( (*it_b)->prevBend, *it_b );
            break;
        }
        ++it_b;
    }
   
}

void Network::depolymerize(int i, int j)
{ depolymerize( vertices[i], vertices[j]); }

void Network::addBend(Vertex *mid, Edge *a, Edge *b)
{ mid->bends.push_back( new Bend(mid, a, b, mid->bends.size() ) ); }

void Network::addBend(int mid, int va, int vb)
{ addBend( vertices[mid], vertices[va], vertices[vb]); }

void Network::addBend(Vertex *mid, Vertex *va, Vertex *vb)
{

    // in edges of mid, find mid-va and mid-vb 
    std::vector<Edge*>::iterator it_e = mid->edges.begin();
    Edge *a = nullptr;
    Edge *b = nullptr;
    while( it_e != mid->edges.end() ) {
        if( (*it_e)->to == va ) a = *it_e;
        if( (*it_e)->to == vb ) b = *it_e;
        if( a != nullptr and b != nullptr ) {
            addBend(mid, a, b);
            break;
        }
        ++it_e;
    }
    // ERROR: NO EDGE ADDED
}

void Network::deleteBend( std::vector<Bend*>::iterator it_bend)
{
    depolymerize( *it_bend);
    // move last bend pointer to current location in the bend list
    Bend *temp = *it_bend;
    *it_bend = temp->mid->bends.back();
    (*it_bend)->index = temp->index;
    temp->mid->bends.pop_back();
    delete temp;
    
}

void Network::deleteBend(Bend *bend)
{
    if( bend == nullptr ) return;

    depolymerize(bend); 

    //std::vector<Bend*>::iterator it_b = std::find(bend->mid->bends.begin(), bend->mid->bends.end(), bend);
    //if( it_b == bend->mid->bends.end() ) return;
    //deleteBend( it_b);
   
    deleteBend( bend->mid->bends.begin() + bend->index ); 

}


void Network::deleteBend(Vertex *mid, Vertex *va)
{
    std::vector<Bend*>::iterator it_b = mid->bends.begin();
    while( it_b != mid->bends.end() ) {
        if( (*it_b)->a->to == va or (*it_b)->b->to == va ) {
            deleteBend( it_b);
            break;
        }
        ++it_b;
    }
    
}

void Network::deleteBend(int mid, int va)
{ deleteBend( vertices[mid], vertices[va]); }


void Network::deleteAllBend(int vi, int vj)
{ deleteBend( vertices[vi], vertices[vj]); }

void Network::deleteAllBend(Vertex *vi, Vertex *vj)
{
    deleteBend(vi,vj);
    deleteBend(vj,vi);
}

void Network::addEdge(int i, int j, int xb, int yb, double l0)
{ addEdge( vertices[i], vertices[j], xb, yb, l0); }

void Network::addEdge(Vertex *vi, Vertex *vj, int xb, int yb, double l0)
{
    // check if already exists??
    vi->edges.push_back( new Edge(vi,vj,xb,yb,l0) );
    vj->edges.push_back( new Edge(vj,vi,-xb,-yb,l0) );

}

void Network::deleteEdge(int vi, int vj)
{ deleteEdge(vertices[vi], vertices[vj]); }

void Network::deleteEdge(Vertex *vi, Vertex *vj)
{
    // find the edge from vi to vj
    std::vector<Edge*>::iterator it_e = vi->edges.begin();
    while( it_e != vi->edges.end() ) {
        if( (*it_e)->to == vj ) {
            deleteEdge(it_e);
            break;
        }
        ++it_e;
    }
    it_e = vj->edges.begin(); 
    while( it_e != vj->edges.end() ) {
        if( (*it_e)->to == vi ) {
            deleteEdge(it_e);
            break;
        }
        ++it_e;
    }
    
}

void Network::deleteEdge( std::vector<Edge*>::iterator it)
{
    Edge *temp = *it;
    *it = temp->from->edges.back();
    temp->from->edges.pop_back();
    delete temp;
}

Network::Vertex::~Vertex()
{
    //delete edges
    for( std::vector<Edge*>::size_type ei=0;ei<edges.size(); ++ei) {
        delete edges[ei];
        edges[ei] = nullptr;
    }
    //delete bends
    for( std::vector<Bend*>::size_type bi=0;bi<bends.size(); ++bi) {
        delete bends[bi];
        bends[bi] = nullptr;
    }
}

void Network::showAdj() const
{
    std::vector<Vertex*>::const_iterator it_v = vertices.begin();
    std::vector<Edge*>::const_iterator it_e;
    while( it_v != vertices.end() ) {
        std::cout << (*it_v)->index << ":\t";
        it_e = (*it_v)->edges.begin();
        while( it_e != (*it_v)->edges.end() ) {
            std::cout << (*it_e)->to->index << '\t';
            ++it_e;
        } 
        std::cout << '\n';
        ++it_v;
    }
}

void Network::showBends() const
{
    std::vector<Vertex*>::const_iterator it_v = vertices.begin();
    std::vector<Bend*>::const_iterator it_b;
    while( it_v != vertices.end() ) {
        std::cout << (*it_v)->index << ":\n";
        it_b = (*it_v)->bends.begin();
        while( it_b != (*it_v)->bends.end() ) {
            std::cout << '\t';
            std::cout << (*it_b)->a->to->index << '\t';
            std::cout << (*it_b)->mid->index << '\t';
            std::cout << (*it_b)->b->to->index << '\n';
            ++it_b;
        }
        ++it_v;
    }
}

std::vector<std::vector<int> > Network::getEdges() const
{
    std::vector<std::vector<int> > edges;
    std::vector<int> edge(4);

    std::vector<Vertex*>::const_iterator it_v = vertices.begin();
    std::vector<Edge*>::const_iterator it_e;
    while( it_v != vertices.end() ) {
        it_e = (*it_v)->edges.begin(); 
        while( it_e != (*it_v)->edges.end() ) {
            if( (*it_e)->to->index > (*it_e)->from->index ) {
                edge[0] = (*it_e)->from->index; 
                edge[1] = (*it_e)->to->index; 
                edge[2] = (*it_e)->xb;
                edge[3] = (*it_e)->yb;
                edges.push_back(edge);
            }
            ++it_e;
        }
        ++it_v;
    }
    return edges;
}

double Network::averageConnectivity() const
{
    int Nv = 0; // number of vertices with at least one bond
    double avgc = 0;
    
    std::vector<Vertex*>::const_iterator it_v = vertices.begin();
    while( it_v != vertices.end() ) {
        avgc += (*it_v)->edges.size();
        if( (*it_v)->edges.size() > 0 ) Nv+=1;
        ++it_v;
    }

    return avgc/Nv;

}


Network::Bend* Network::firstBend( Bend *bend) const
{
    if( bend == nullptr ) return nullptr;
    Bend *first = bend;
    while( bend->prevBend != nullptr) {
        bend = bend->prevBend;
        if( bend == first ) break;
    }
    return bend;
}

std::vector<std::vector<int> > Network::getPolymers() 
{
    std::vector<std::vector<int> > polymers;
    polymers.push_back( std::vector<int>() );

    resetFilamentIndex();

    int filament_index = 0;
    std::vector<Vertex*>::const_iterator it_v = vertices.begin();
    std::vector<Bend*>::const_iterator it_b;
    while( it_v != vertices.end() ) {
        it_b = (*it_v)->bends.begin(); 
        while( it_b != (*it_v)->bends.end() ) {
            if( (*it_b)->filament == -1 ){
                Bend *bend = *it_b;            
                Bend *first = firstBend(bend);
                bend = first;
                while( bend != nullptr) {
                    // set filament index
                    bend->filament = filament_index;

                    // if both nextBend and prevBend are 0, it is a filament with a single bend
                    if( bend->nextBend == nullptr and bend->prevBend == nullptr ) {
                        polymers[filament_index].push_back( bend->a->to->index );
                        polymers[filament_index].push_back(bend->mid->index);
                        polymers[filament_index].push_back( bend->b->to->index );
                        break;
                    }

                    // if it is the first bend that is not of a loop filament, also include the first vertex
                    if( bend == first and first->prevBend == nullptr) {
                        polymers[filament_index].push_back( bend->previousVertex()->index );
                    }

                    // add vertex index
                    polymers[filament_index].push_back(bend->mid->index);

                    // if it is the last bend, also include the next vertex index
                    if( bend->nextBend == nullptr or bend->nextBend == first ) {
                        polymers[filament_index].push_back( bend->nextVertex()->index );
                        break;
                    }

                    bend = bend->nextBend;
                }

                polymers.push_back( std::vector<int>() );
                ++filament_index;
            }
            ++it_b;
        }
        ++it_v;
    }

    return polymers;
}

void Network::resetFilamentIndex()
{
    std::vector<Vertex*>::const_iterator it_v = vertices.begin();
    std::vector<Bend*>::const_iterator it_b;
    while( it_v != vertices.end() ) {
        it_b = (*it_v)->bends.begin(); 
        while( it_b != (*it_v)->bends.end() ) {
            (*it_b)->filament = -1;
            ++it_b;
        }
        ++it_v;
    }   
}


void Network::showPolymers()
{
    std::vector<std::vector<int> > p = getPolymers();
    std::vector<std::vector<int> >::iterator it_p = p.begin();
    while( it_p != p.end() ) {
        std::vector<int>::iterator it = it_p->begin();
        while( it != it_p->end() ) {
            std::cout << *it << '\t';
            ++it;
        }
        std::cout << '\n';
        ++it_p;
    }

}


Network::Vertex* Network::Bend::previousVertex() const
{
    if( prevBend != nullptr ) return prevBend->mid;

    // if bend is not part of filament, return nullptr
    if( nextBend == nullptr ) return nullptr;
    if( a->to == nextBend->mid ) return b->to;
    return a->to;
}

Network::Vertex* Network::Bend::nextVertex() const
{
    if( nextBend != nullptr ) return nextBend->mid;

    // if bend is not part of filament, return nullptr
    if( prevBend == nullptr ) return nullptr;

    if( a->to == prevBend->mid ) return b->to;
    return a->to;
}

#endif
