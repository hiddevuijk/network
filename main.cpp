
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

    int Nx = 50;
    int Ny = Nx;
    double Lx = Nx;
    double Ly = Lx*sqrt(3/4.);
    double z = 3.2;
	double kappa = 1.e-2;
	long int seed  = 333356789;
	double sigma = 0;
	int n = 0;
	
    double gamma = 0;
    double gmax = 1.2e1;
	double dg0 = 0.00;
    double dg = 0.00025;
	double alpha = 1.05;

	bool backwards = false;

	boost::mt19937 rng(2*seed);

    Graph graph = generateGraph(Nx,Ny,Lx,z, seed);
	Network network(graph,Lx,Ly, kappa);

    ofstream top("topology.txt");
    graph.write(top);
    top.close();


    ofstream out0("r0.dat");
    network.savePositions(out0);
    out0.close();

	network.shearAffine(dg0);
	gamma += dg0;
	double e, e1;
    while( gamma < gmax ) {
        gamma += dg;

		network.shearAffine(dg);
		//network.shear(dg);
		e = network.totalEnergy();

		for(int i=0;i<n; ++i) {
			//network.shake(rng, sigma);
			network.minimize();
			e1 = network.totalEnergy();
			e = min(e,e1);
		}

		cout << gamma <<'\t' << e << endl;;
		dg *= alpha;
    }

	if(backwards) cout << gamma <<'\t' << e << endl;;
    while( gamma > dg/alpha and backwards ) {
		dg /= alpha;
        gamma -= dg;

		//network.shake(rng, sigma);
		network.shear(-dg);
		e = network.totalEnergy();

		for(int i=0;i<n; ++i) {
			network.shake(rng, sigma);
			network.minimize();
			e1 = network.totalEnergy();
			e = min(e,e1);
		}


		cout << gamma <<'\t' << e << endl;;
	}
    ofstream out("r.dat");
    network.savePositions(out);
    out.close();



    return 0;

}

