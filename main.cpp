
#include "graph.h"
#include "generate_graph.h"
#include "network.h"

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
    
    int Nx = 6;
    int Ny = Nx;
    double Lx = Nx;
    double Ly = Lx*sqrt(3/4.);
    double z = 3.4;
	double kappa = 1.;

    Graph graph = generateGraph(Nx,Ny,Lx,z);
    
    Network network(graph,Lx,Ly, kappa);
	double y = gsl_vector_get(network.r, 2*8+1);
	gsl_vector_set(network.r, 2*8+1, y+0.3);
	network.set_phi0();

	for(unsigned int bi=0;bi < network.bends.size(); ++bi) {
		cout << network.bends[bi].i << '\t';
		cout << network.bends[bi].j << '\t';
		cout << network.bends[bi].k << endl;
		cout << network.bends[bi].phi0 << endl << endl;
	}

    ofstream top("topology.txt");
    graph.write(top);
    top.close();


    ofstream out0("r0.dat");
    network.savePositions(out0);
    out0.close();


	return 0;

    double gamma = 0;
    double gmax = 1.;
    double dg = 0.001;

	double e, e1, e2, e3;
    while( gamma < gmax ) {
        gamma += dg;

		network.shear(dg);
		e1 = network.totalEnergy();
		
		e = e1;
		network.shear(0.1*dg);
		network.shear(-0.1*dg);
		e2 = network.totalEnergy();
		
		network.shear(-0.01*dg);
		network.shear(0.01*dg);
		e3 = network.totalEnergy();

		e = min( e1, min(e2,e3) );

		//for(int i=0; i<5; ++i) {
		//	network.shear(-0.05*dg);
		//	network.shear(0.05*dg);
		//	e = min( e, network.totalEnergy() );
		//}


        //network.shear(dg);
        cout << gamma <<'\t' << e << endl;;
		//if( gamma > 0.3  ) dg *= 1.05;
		dg *= 1.01;
	
    }
    ofstream out("r.dat");
    network.savePositions(out);
    out.close();


    return 0;

}

