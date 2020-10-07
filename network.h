#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H

#include <vector>


struct Spring
{
    int I1, I2;
    double l0;
};

struct Bend
{
    int I1, I2, I3;
    double theta0;
};

class Network
{
  public:
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
