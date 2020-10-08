#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H

#include <vector>


struct Spring
{
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

class Network
{
  public:
    Network() {};
    Network(int Nspring, int Nbend)
    : Nspring(Nspring), springs( std::vector<Spring>(Nspring) ),
      Nbend(Nbend)    ,bends( std::vector<Bend>(Nbend) ) 
    {}

    void shear(double dDelta);


  //private:
    // netowrk dimensions
    double Lx, Ly;
    // shear
    double delta;

    int Nspring;
    std::vector<Spring> springs;
    int Nbend;
    std::vector<Bend> bends;

};



#endif
