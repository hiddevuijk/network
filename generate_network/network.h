#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H

#include <vector>
#include <math.h>


struct Spring
{
    Spring(): I1(-1), I2(-1) {}
    Spring( int I1, int I2, double l0, int XB, int YB)
    : I1(I1), I2(I2), l0(l0), XB(XB), YB(YB) {}

    // spring connection between node I1 and I2
    int I1, I2;
    // rest length of the spring
    double l0;
    // if node[I1] -> node[I2] crosses X boundary from left to right, XB=  1
    //                                                 right to left, XB= -1
    // if node[I1] -> node[I2] crosses Y boundary from down to up, YB=  1
    //                                                 up to down, YB= -1
    int XB, YB;
};

struct Bend
{
    // bending between node I1,I2 and I3
    int I1, I2, I3;
    // rest angle
    double theta0;
    // boundaries ????
};

struct Node
{
    Node(): I(-1){};
    int I;
    std::vector<Spring> springs;
    std::vector<Bend> bends;
    double x,y;

    void add_spring( int I1, int I2, double l0, int XB, int YB) {
        springs.push_back( Spring( I1, I2, l0, XB, YB) );
    }

};

class Network
{
  public:
    Network() {};
    Network(int Nx, int Ny, double Lx, double Ly);
    Network(int Nspring, int Nbend)
    : Nspring(Nspring), springs( std::vector<Spring>(Nspring) ),
      Nbend(Nbend)    ,bends( std::vector<Bend>(Nbend) ) 
    {}

    void shear(double dDelta);

    void save_network( std::ofstream& out );
  //private:
    // netowrk dimensions
    double Lx, Ly;
    // shear
    double delta;

    int Nspring;
    std::vector<Spring> springs;
    int Nbend;
    std::vector<Bend> bends;

    std::vector<Node> nodes; 
    std::vector<double> r;
    
};


Network::Network( int Nx, int Ny, double Lx, double Ly)
{
    double dx = 1; 
    double dy = dx*sqrt(3.)/2.;
    int Inode = 0;
    for(int xi=0; xi<Nx; ++xi) {
        // add first row of nodes
        nodes.push_back(Node());
        nodes[Inode].I = Inode;
        ++Inode;
        // connect  to node on the left
        if( xi >0 ) {
            nodes[Inode].add_spring(Inode, Inode-1, 1., 0, 0); 
            nodes[Inode-1].add_spring(Inode-1, Inode, 1., 0, 0); 
        }
        // connect fist node with the last node
        if( xi == Nx-1) {
            nodes[Inode].add_spring(Inode, Inode-Nx+1, 1., 1, 0);
            nodes[Inode-Nx+1].add_spring(Inode-Nx+1, Inode, 1., -1, 0);
        }
        
        //for(int yi=0; yi<Ny; ++yi) {
    
        //  r.push_back( xi*dx); // x position
        //  r.push_back( yi*dy); // y position
        //}
    }


}

void Network::save_network( std::ofstream& out )
{
    int Nnodes = nodes.size();
    for( int i=0;i<Nnodes; ++i) {
        Node node = nodes[i];
        int I1 = node.I1;
        int I2 = node.I2;
        

    }

}
#endif
