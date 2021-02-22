
#include "graph.h"
#include "generate_graph.h"
#include "network.h"

#include "boost/random.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

double min( double a, double b) {
	if( a < b ) return a;
	else return b;
}



int main()
{

    int Nx = 30;
    int Ny = Nx;
    double Lx = Nx;
    double Ly = Lx*sqrt(3/4.);
    double z = 3.2;
	double kappa = 0*1.e-2;
	long int seed  = 113356789;
	
    double gamma = 0;
    double gmax = 1.2e1;
    double dg = 0.0005;
	double alpha = 1.05;


	double eLine = 0.1;
	double dLine = 0.1;
	double e = 1e-9;

	bool backwards = true;

	boost::mt19937 rng(2*seed);

    Graph graph = generateGraph(Nx,Ny,Lx,z, seed);
	Network network(graph,Lx,Ly, kappa);

    ofstream top("topology.txt");
    graph.write(top);
    top.close();


    ofstream out0("r0.dat");
    network.savePositions(out0);
    out0.close();

	double energy;
    while( fabs(gamma) < gmax ) {
        gamma += dg;

		network.shearAffine(1.25dg,eLine, dLine, e*1000);
		network.shearAffine(-0.25*dg,eLine, dLine, e);
		energy = network.totalEnergy();

		cout << gamma <<'\t' << energy << endl;;
		dg *= alpha;
    }

	if(backwards) cout << gamma <<'\t' << e << endl;;
    while( fabs(gamma) > fabs(dg/alpha) and backwards ) {
		dg /= alpha;
        gamma -= dg;

		network.shear(-dg, eLine, dLine, e);
		energy = network.totalEnergy();


		cout << gamma <<'\t' << energy << endl;;
	}
    ofstream out("r.dat");
    network.savePositions(out);
    out.close();



    return 0;

}

