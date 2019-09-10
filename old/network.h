#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H

#include <fstream>
#include <vector>
#include <string>

using namespace std;


class Node {
public:
    Node() : x(0), y(0),id(-1), connections(std::vector<Node*>()),
        boundary(std::vector<char>()) {}
    Node(double x, double y,int id, std::vector<Node*> c, 
        std::vector<char> b) : x(x), y(y),id(id), connections(c), boundary(b) {}

    double x,y;
    int id;
    std::vector<Node*> connections;
    std::vector<char> boundary;
    
};


class Network{
private:

    int N;
    int Nangles;
    std::vector<Node> nodes; 
    std::vector<std::vector<Node*> > angles;
    

public:

    Network():N(0){}

    void add_node(double x, double y, int id, std::vector<Node*> c, std::vector<char> b);
    void remove_node(Node* node);
    void split_node(Node* node);

    int get_N() const { return N;}

    Node* get_last_node_ref() { return &(nodes[nodes.size()-1]); }
    void connect(int i, int j, char b) {
            nodes[i].connections.push_back(&nodes[j]);
            nodes[i].boundary.push_back(b);
    }

    void write_positions(string filename) {
        ofstream out(filename.c_str());
        for(int i=0;i<N;++i) {
            out << nodes[i].x << '\t'
                << nodes[i].y << '\t';
            for(int j=0;j<nodes[i].connections.size() ; ++j) 
                out << nodes[i].connections[j]->id << '\t';
            if(i< (N*N-1) ) out << '\n';
        }
    }
};

void Network::add_node(double x, double y,int id, std::vector<Node*> c,
   std::vector<char> b)
{

    nodes.push_back(Node(x, y,id, c, b));
    N = nodes.size();
}



#endif
