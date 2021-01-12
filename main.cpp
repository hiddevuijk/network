
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
    
    int Nx = 10;
    int Ny = Nx;
    double Lx = Nx;
    double Ly = Lx*sqrt(3/4.);
    double z = 3.4;

    Graph graph = generateGraph(Nx,Ny,Lx,z);
    
    Network network(graph);
    network.set_k(1.);
    network.set_Lx(Lx);
    network.set_Ly(Ly);

    double gamma = 0;
    double gmax = 4.;
    double dg = 1e-3;

	double e, e1, e2, e3;
    while( gamma < gmax ) {
        gamma += dg;

		network.shear(dg);
		e1 = network.totalEnergy();

		network.shear(0.1*dg);
		network.shear(-0.1*dg);
		e2 = network.totalEnergy();
		
		network.shear(-0.01*dg);
		network.shear(0.01*dg);
		e3 = network.totalEnergy();

		e = min( e1, min(e2,e3) );

		for(int i=0; i<2; ++i) {
			network.shear(-0.05*dg);
			network.shear(0.05*dg);
			e = min( e, network.totalEnergy() );
		}


        //network.shear(dg);
        cout << gamma <<'\t' << e << endl;;
		if( gamma > 0.3 ) dg *= 1.1;
		if( gamma > 0.4 ) dg *= 1.25;
		//if( gamma > 0.175 and gamma < 0.185 ) dg = 1e-5;
		//if( gamma > 0.185 and gamma < 0.19 ) dg = 1.e-3;
	
    }



    ofstream top("topology.txt");
    graph.write(top);
    top.close();

    ofstream out("r.dat");
    network.savePositions(out);
    out.close();


    return 0;

}

