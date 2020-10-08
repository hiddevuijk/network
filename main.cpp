
#include "network.h"
#include "energy.h"
#include "system.h"

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>

#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


int main()
{

    int N = 3;
    vector<double> rInit(2*N);
    rInit[0] = 0.0;  rInit[1] = 0.0;
    rInit[2] = 1.0;  rInit[3] = 1.0;
    rInit[4] = 2.0;  rInit[5] = 0.0;


    int Nspring = 3;
    int Nbend = 0;

    Network network(Nspring, Nbend);
    network.springs[0].I1 = 0;
    network.springs[0].I2 = 1;
    network.springs[0].l0 = 1.;
    
    network.springs[1].I1 = 0;
    network.springs[1].I2 = 2;
    network.springs[1].l0 = 1.;

    network.springs[2].I1 = 1;
    network.springs[2].I2 = 2;
    network.springs[2].l0 = 1.;



    NetworkMinimizer network_minimizer(N, rInit, &network);
    network_minimizer.minimize();



    return 0;
}

