
#include "graph.h"
#include "generate_graph.h"
#include "network.h"

#include "ConfigFile.h"

#include "boost/random.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{

	ConfigFile config("input.txt");

	int Nx = config.read<int>("Nx");
    int Ny = Nx;
	// assert( Ny % 2 == 0 );
    double Lx = config.read<double>("Lx");
    double Ly = Lx*sqrt(3/4.);
    double z = config.read<double>("z");
	double kappa = config.read<double>("kappa");
	long int seed  = config.read<long int>("seed");
	
    double gmax = config.read<double>("gmax");
    double dg = config.read<double>("dg");
    double gamma = 0;
	double alpha = config.read<double>("alpha");


	double e = config.read<double>("e");
	double dt0 = config.read<double>("dt0");
	double dtmax = config.read<double>("dtmax");
	double m = config.read<double>("m");

	string topologyName = config.read<string>("topologyName");
	string r0Name = config.read<string>("r0Name");
	string rName = config.read<string>("rName");
	string gammaEName = config.read<string>("gammaEName");

    Graph graph = generateGraph(Nx,Ny,Lx,z, seed);
	Network network(graph,Lx,Ly, kappa);


    ofstream top(topologyName);
    graph.write(top);
    top.close();


    ofstream out0(r0Name);
    network.savePositions(out0);
    out0.close();

	ofstream gEout( gammaEName );

	double Hs, Hb;
	vector<double> sigma(4);
	int i=0;


    while( fabs(gamma) < gmax ) {
		
        gamma += dg;

		//network.shearAffine(dg);
		//network.stretchX(dg);
		//network.stretchY(dg);
		network.shearAffine(dg);
		network.minimize(e, dt0, dtmax, m);

		Hs = network.edgeEnergy();	
		Hb = network.bendEnergy();	
		//cout << network.get_forceNorm() << endl;
		if(i%10 == 0 ){
			cout << gmax << "\t" << gamma << endl;
		}
		i++;
	

		gEout << gamma << '\t';
		gEout << Hs << '\t';
		gEout << Hb << '\t';
		gEout << sigma[0] << '\t';
		gEout << sigma[1] << '\t';
		gEout << sigma[2] << '\t';
		gEout << sigma[3] << endl;

		dg *= alpha;
    }

    ofstream out(rName);
    network.savePositions(out);
    out.close();



    return 0;

}

