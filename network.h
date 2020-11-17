#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H


/*
    to do:
            -- destructors
            -- throw error when polymerizing nodes without a bend
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

    void addEdge(int i, int j);
    void deleteEdge(int i, int j);

    void addBend(int mid, int va, int vb);
    void deleteBend(int mid, int va);
    void deleteAllBend(int vi, int vj); // delete all Bends with edge vi-vj

    void polymerize(int i, int j);
    void depolymerize(int i, int j); // test 
    void depolymerize(int i); // test // remove from all polymers

    // this changes the indices!!!
    void removeUnconectedVertices(); // --
    // ....

    std::vector<std::vector<int> > getEdges() const;
    std::vector<std::vector<int> > getBends() const; // ---
    std::vector<std::vector<int> > getPolymers() const; // ---

    void showAdj() const;
    void showBends() const;
    // print polymer starting at i through j
    void showPolymer(int i, int j) const;


    class Vertex {
      public:
        Vertex(): index(-1) {}
        Vertex(int i) : index(i) {}
        Vertex(int i, double x, double y) : index(i), r(x,y) {}
        // destructor ----
        int index;
        std::vector<Edge*> edges;
        std::vector<Bend*> bends;

        // add location info
        vec2 r;
    };

    class Edge {
      public:
        Vertex *from, *to;
        int xBoundary, yBoundary;
        double l0; 
    };


    class Bend {
      public:
        Bend()
            : mid(nullptr), a(nullptr), b(nullptr),
              prevBend(nullptr), nextBend(nullptr),
              filament(-1) {}

        Bend( Vertex *mid, Edge* a, Edge *b)
            : mid(mid), a(a), b(b),
              prevBend(nullptr), nextBend(nullptr),
              filament(-1) {}
        Vertex *mid;
        Edge *a, *b;
        Bend *prevBend, *nextBend;
        int filament;
        double theta0;
    };

    private:
        std::vector<Vertex*> vertices;

        void deleteVertex(int i);
        void deleteVertex(Vertex *vi);
        void exchangeVertex(int i, int j);
        void exchangeVertex(Vertex *vi, Vertex *vj);

        void addEdge(Vertex *vi, Vertex *vj);
        void deleteEdge(Vertex *vi, Vertex *vj);

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
{ mid->bends.push_back( new Bend(mid, a, b) ); }

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
    temp->mid->bends.pop_back();
    delete temp;
    
}

void Network::deleteBend(Bend *bend)
{
    if( bend == nullptr ) return;

    depolymerize(bend); 

    std::vector<Bend*>::iterator it_b = std::find(bend->mid->bends.begin(), bend->mid->bends.end(), bend);
    if( it_b == bend->mid->bends.end() ) return;
    deleteBend( it_b);
    

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

#endif
