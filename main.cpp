
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
    
    int Nx = 8;
    int Ny = Nx;
    double Lx = Nx;
    double Ly = Lx*sqrt(3/4.);
    double z = 3.4;
	double kappa = 0.;

    Graph graph = generateGraph(Nx,Ny,Lx,z);
    
    Network network(graph, kappa);
    network.set_Lx(Lx);
    network.set_Ly(Ly);

    double gamma = 0;
    double gmax = 5.;
    double dg = 0.01;

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
		//if( gamma > 0.3  ) dg *= 1.05;
		//if( gamma > 0.35  ) dg *= 1.1;
	
    }



    ofstream top("topology.txt");
    graph.write(top);
    top.close();

    ofstream out("r.dat");
    network.savePositions(out);
    out.close();


    return 0;

}

