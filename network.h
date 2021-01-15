#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H

/*
to do:
    -- write EdEnergy
    -- destructors

	-- phi in radians or degrees??

	-- change bend::energy_dEnergy
*/

#include "vec2.h"
#include "graph.h"
#include "generate_graph.h"

#include "gsl/gsl_multimin.h"

#include <math.h>


double energy( const gsl_vector *v, void *params);
void dEnergy( const gsl_vector *v, void *params, gsl_vector *df);
void EdE( const gsl_vector *x, void *params, double *f, gsl_vector *df);

class Network
{
  private:
    class Edge;
    class Bend;

  public:
    Network(const Graph&, double kappa);

    void minimize();
    void shear(double delta_gamma); 
    void shearAffine(double delta_gamma); 
    int get_Nv() const {return Nv; }
    int get_Nedges() const { return Ne; }
    int get_Nbends() const { return Nb; }

    double totalEnergy() const; 
    double get_edgeEnergy( int ei, const gsl_vector *r) const
        { return edges[ei].energy(r, this); }

    void get_edgeDEnergy( int ei, const gsl_vector *r, gsl_vector *df) const
        {  edges[ei].dEnergy(r, df, this); }

    double get_edgeEdE( int ei, const gsl_vector *r, gsl_vector *df) const
        { return edges[ei].energy_dEnergy(r, df, this); }

	double get_bendEnergy( int bi, const gsl_vector *r) const
		{ return bends[bi].energy(r, this); }

	void get_bendDEnergy( int bi, const gsl_vector *r, gsl_vector *df) const
		{ bends[bi].dEnergy(r, df, this); }

	double get_bendEdE( int bi, const gsl_vector *r, gsl_vector *df) const 
		{ return bends[bi].energy_dEnergy(r ,df, this); }


    std::vector<double> get_positions() const;
    void savePositions(std::ostream& out) const;

    void set_Lx(double Lxx);
    void set_Ly(double Lyy);
  private:
    class Edge{
      public:
        int i,j; 
        int xb, yb;
        double l0; 

		double get_l( const gsl_vector *r, const Network *net);
        double energy(const gsl_vector *r, const Network *net) const;
        void dEnergy( const gsl_vector *r,  gsl_vector *df, const Network *net) const;
        double energy_dEnergy(const gsl_vector *r, gsl_vector *df, const Network *net) const;
    };

    class Bend{
	  public:
		int i,j,k;
		int xib, yib;
		int xjb, yjb;

		double kappa;
		double phi0;

		double get_lji( const gsl_vector *r, const Network *net) const;
		double get_ljk( const gsl_vector *r, const Network *net) const;
		double get_phi( const gsl_vector *r, const Network *net) const;

		double energy( const gsl_vector *r, const Network *net) const;
		void dEnergy ( const gsl_vector *r, gsl_vector *df, const Network *net) const;
		double energy_dEnergy( const gsl_vector *r, gsl_vector *df, const Network *net) const;
    };


	void set_kappa();

    const gsl_multimin_fdfminimizer_type *T;
    gsl_multimin_fdfminimizer *s;

    gsl_vector *r;
    gsl_multimin_function_fdf functions;

    std::vector<Edge> edges;
    std::vector<Bend> bends;
  public:
    double kappa;
    double Lx, Ly;
    int Nv,Ne,Nb;

    double gamma;
    int maxIter = 1000000;
};

Network::Network(const Graph& g, double kappaa)
{
    
    Nv = g.Nvertices();

    //T = gsl_multimin_fdfminimizer_conjugate_fr;
    //T = gsl_multimin_fdfminimizer_conjugate_pr;
    //T = gsl_multimin_fdfminimizer_vector_bfgs;
    T = gsl_multimin_fdfminimizer_vector_bfgs2;
    //T = gsl_multimin_fdfminimizer_steepest_descent;

    s = gsl_multimin_fdfminimizer_alloc(T,2*Nv);

    r = gsl_vector_alloc(2*Nv);

    functions.n = 2*Nv;
    functions.f = &energy;
    functions.df = &dEnergy;
    functions.fdf = &EdE;
    functions.params = this;

    for(int vi=0; vi<Nv; ++vi ) {
        vec2 p = g.getVertexPosition(vi);
        gsl_vector_set( r, 2*vi  , p.x);   
        gsl_vector_set( r, 2*vi+1, p.y);   
    }

    std::vector<std::vector<int> > e = g.getEdges();
    Ne = e.size();
    edges = std::vector<Edge>(Ne);
    for(int ei=0; ei< Ne; ++ei ) {
        edges[ei].i = e[ei][0]; 
        edges[ei].j = e[ei][1]; 
        edges[ei].xb = e[ei][2]; 
        edges[ei].yb = e[ei][3]; 
        edges[ei].l0 = e[ei][4]; 
    }

	kappa = kappaa;
	//set_kappa(r);
}


void Network::shearAffine( double delta_gamma )
{
    gamma += delta_gamma;
    double x,y;
    //affine deformation
    for( int vi=0; vi<Nv; ++vi ) {
        x = gsl_vector_get(r,2*vi);
        y = gsl_vector_get(r,2*vi+1);
        gsl_vector_set( r, 2*vi, x+delta_gamma*y);
    }

    minimize();
}


void Network::shear( double delta_gamma )
{
    gamma += delta_gamma;
    minimize();
}

void Network::minimize()
{
    // add params to network
    gsl_multimin_fdfminimizer_set(s, &functions, r, 1e-1, .01);

    int iter = 0;
    int status;
    do{
        iter++;    
        status = gsl_multimin_fdfminimizer_iterate(s);
        if( status ) break;
        status = gsl_multimin_test_gradient( s->gradient, 1e-12);
        
    } while( status == GSL_CONTINUE && iter < maxIter);
    if( iter >= maxIter ) std::cout << "\t Fuck \n";

    // copy to r
    gsl_vector * x = gsl_multimin_fdfminimizer_x(s);
    for(int i=0;i<2*Nv; ++i) {
        gsl_vector_set(r,i, gsl_vector_get(x,i) );
    }
}



std::vector<double> Network::get_positions() const
{
    std::vector<double> positions(2*Nv);
    for(int i=0; i<2*Nv; ++i) {
        positions[i] = gsl_vector_get(r,i);
    }

    return positions;
}

void Network::savePositions(std::ostream& out) const
{
    for(int i=0; i<Nv; ++i) {
        out << gsl_vector_get(r, 2*i) << '\t';
        out << gsl_vector_get(r, 2*i+1) << '\n';
    }
}

double Network::totalEnergy() const
{
    double e = 0;
    for(int i=0;i<Ne;++i) {
        e += get_edgeEnergy(i,r);
    }
    return e;
}

double Network::Edge::get_l( const gsl_vector *r, const Network *net)
{

    double g = net->gamma*net->Ly;
    double dx = gsl_vector_get(r, 2*i) - gsl_vector_get(r,2*j) - (net->Lx)*xb - yb*g;
    double dy = gsl_vector_get(r, 2*i+1) - gsl_vector_get(r,2*j+1) - (net->Ly)*yb;

    return std::sqrt( dx*dx + dy*dy);
}
double Network::Edge::energy(const gsl_vector *r,const Network *net) const
{
    double g = net->gamma*net->Ly;
    double dx = gsl_vector_get(r, 2*i) - gsl_vector_get(r,2*j) - (net->Lx)*xb - yb*g;
    double dy = gsl_vector_get(r, 2*i+1) - gsl_vector_get(r,2*j+1) - (net->Ly)*yb;


    double l = std::sqrt( dx*dx + dy*dy);
    l -= l0;
    return 0.5*l*l;
}

void Network::Edge::dEnergy( const gsl_vector *r, gsl_vector *df, const Network *net) const
{
    double g = net->gamma*net->Ly;
    double dx = gsl_vector_get(r, 2*i) - gsl_vector_get(r,2*j) - (net->Lx)*xb - yb*g;
    double dy = gsl_vector_get(r, 2*i+1) - gsl_vector_get(r,2*j+1) - (net->Ly)*yb;
    double l = std::sqrt( dx*dx + dy*dy);
	
    dx *= 1-l0/l;
    dy *= 1-l0/l;

    double temp = gsl_vector_get(df, 2*i) + dx;
    gsl_vector_set(df, 2*i, temp );

    temp = gsl_vector_get(df, 2*j) - dx;
    gsl_vector_set(df, 2*j, temp );

    temp = gsl_vector_get(df, 2*i+1) + dy;
    gsl_vector_set(df, 2*i+1, temp);

    temp = gsl_vector_get(df, 2*j+1) - dy;
    gsl_vector_set(df, 2*j+1, temp);

}

double Network::Edge::energy_dEnergy(const gsl_vector *r, gsl_vector *df, const Network *net) const
{

    double g = net->gamma*net->Ly;
    double dx = gsl_vector_get(r, 2*i) - gsl_vector_get(r,2*j) - (net->Lx)*xb - yb*g;
    double dy = gsl_vector_get(r, 2*i+1) - gsl_vector_get(r,2*j+1) - (net->Ly)*yb;
    double l = std::sqrt( dx*dx + dy*dy);

    dx *= 1-l0/l;
    dy *= 1-l0/l;

    double temp = gsl_vector_get(df, 2*i) + dx;
    gsl_vector_set(df, 2*i, temp );

    temp = gsl_vector_get(df, 2*j) - dx;
    gsl_vector_set(df, 2*j, temp );

    temp = gsl_vector_get(df, 2*i+1) + dy;
    gsl_vector_set(df, 2*i+1, temp);

    temp = gsl_vector_get(df, 2*j+1) - dy;
    gsl_vector_set(df, 2*j+1, temp);

    l -= l0;
    return 0.5*l*l;

}

double Network::Bend::get_lji( const gsl_vector *r, const Network *net) const
{
	double xi = gsl_vector_get(r, 2*i);
	double yi = gsl_vector_get(r, 2*i+1);
	
	double xj = gsl_vector_get(r, 2*j);
	double yj = gsl_vector_get(r, 2*j+1);

	double dxji = xj - xi;
	double dyji = yj - yi;

	return std::sqrt( dxji*dxji + dyji*dyji );
}

double Network::Bend::get_ljk( const gsl_vector *r, const Network *net) const
{
	double xk = gsl_vector_get(r, 2*k);
	double yk = gsl_vector_get(r, 2*k+1);
	
	double xj = gsl_vector_get(r, 2*j);
	double yj = gsl_vector_get(r, 2*j+1);

	double dxjk = xj - xk;
	double dyjk = yj - yk;

	return std::sqrt( dxjk*dxjk + dyjk*dyjk );
}

double Network::Bend::get_phi( const gsl_vector *r, const Network *net) const
{

	double xi = gsl_vector_get(r, 2*i);
	double yi = gsl_vector_get(r, 2*i+1);
	
	double xj = gsl_vector_get(r, 2*j);
	double yj = gsl_vector_get(r, 2*j+1);

	double xk = gsl_vector_get(r, 2*k);
	double yk = gsl_vector_get(r, 2*k+1);

	double dxji = xj - xi;
	double dyji = yj - yi;
	double dxjk = xj - xk;
	double dyjk = yj - yk;

	double norm = std::sqrt( dxji*dxji + dyji*dyji );
	dxji /= norm;
	dyji /= norm;

	norm = std::sqrt( dxjk*dxjk + dyjk*dyjk );
	dxjk /= norm;
	dyjk /= norm;

	return std::acos( std::sqrt( dxji*dxjk + dyji*dyjk ) );

}

double Network::Bend::energy( const gsl_vector *r, const Network *net) const
{
	double phi =  get_phi(r,net);
	phi -= phi0;

	return kappa*phi0*phi0/2;	

}

void Network::Bend::dEnergy ( const gsl_vector *r, gsl_vector *df, const Network *net) const
{

	
	double dxji = gsl_vector_get(r, 2*i) - gsl_vector_get(r,  2*j);
	double dxjk = gsl_vector_get(r, 2*k) - gsl_vector_get(r,  2*j);
	double dyji = gsl_vector_get(r, 2*i+1) - gsl_vector_get(r,  2*j+1);
	double dyjk = gsl_vector_get(r, 2*k+1) - gsl_vector_get(r,  2*j+1);

	double dji = std::sqrt( dxji*dxji + dyji*dyji);
	double djk = std::sqrt( dxjk*dxjk + dyjk*dyjk);

	// dd = d^_ji*d^_jk
	double dd =  (dxji*dxjk + dyji*dyjk)/(dji*djk);

	double phi = std::acos(dd);
	
	double dphi = -1/std::sqrt( 1 -dd*dd);

	// derivarives of dd
	// d(dd)/dxi
	double ddxi = dxjk/(dji*djk) - dd*dxji/(dji*dji);
	// d(dd)/dyi
	double ddyi = dyjk/(dji*djk) - dd*dyji/(dji*dji);
	// d(dd)dxk
	double ddxk = dxji/(dji*djk) - dd*dxjk/(djk*djk);
	// d(dd)/dyi
	double ddyk = dyji/(dji*djk) - dd*dyjk/(djk*djk);
	// d(dd)/dxj
	double ddxj = -(dxji+dxjk)/(dji*djk) + dd*dxji/(dji*dji);
	// d(dd)/dyj
	double ddyj = -(dyji+dyjk)/(dji*djk) + dd*dyji/(dji*dji);

	double F;
	// set F_xi
	F = gsl_vector_get(df, 2*i);
	gsl_vector_set(df, 2*i, F-kappa*(phi-phi0)*dphi*ddxi);

	// set F_yi
	F = gsl_vector_get(df, 2*i+1);
	gsl_vector_set(df, 2*i+1, F-kappa*(phi-phi0)*dphi*ddyi);

	// set F_xj
	F = gsl_vector_get(df, 2*j);
	gsl_vector_set(df, 2*j, F-kappa*(phi-phi0)*dphi*ddxj);

	// set F_yj
	F = gsl_vector_get(df, 2*j+1);
	gsl_vector_set(df, 2*j+1, F-kappa*(phi-phi0)*dphi*ddyj);

	// set F_xk
	F = gsl_vector_get(df, 2*k);
	gsl_vector_set(df, 2*k, F-kappa*(phi-phi0)*dphi*ddxk);

	// set F_yk
	F = gsl_vector_get(df, 2*k+1);
	gsl_vector_set(df, 2*k+1, F-kappa*(phi-phi0)*dphi*ddyk);


}

double Network::Bend::energy_dEnergy( const gsl_vector *r, gsl_vector *df, const Network *net) const
{
	dEnergy(r,df,net);
	return energy(r,net);
}


void Network::set_Lx(double Lxx)
{ Lx = Lxx; }
void Network::set_Ly(double Lyy)
{ Ly = Lyy; }

/*
    energy functions
*/

double energy( const gsl_vector *v, void *params)
{
    Network *net = (Network *) params;
    double e=0;

	// energy from stretching bonds
    for(int ei=0; ei < net->get_Nedges(); ++ei) {
        e += net->get_edgeEnergy(ei, v);
    }

	// energy from bending
	for(int bi=0; bi < net->get_Nbends(); ++bi) {
		e += net->get_bendEnergy(bi, v);
	}
	

    return e;
}

void dEnergy( const gsl_vector *v, void *params, gsl_vector *df)
{
    
    Network *net = (Network *) params; 
    gsl_vector_scale( df, 0); // set all elements to 0
    for( int ei=0; ei< net->get_Nedges(); ++ei ) {
        net->get_edgeDEnergy(ei, v, df);
    } 

	for( int bi=0; bi < net->get_Nbends(); ++bi) {
		net->get_bendDEnergy(bi, v, df);
	}
}

void EdE( const gsl_vector *v, void *params, double *f, gsl_vector *df)
{
    *f = energy(v,params);
    dEnergy( v, params, df);

    Network *net = (Network *) params;
    double e = 0;
    gsl_vector_scale( df, 0); // set all elements to 0
    for(int ei=0; ei < net->get_Nedges(); ++ei) {
        e += net->get_edgeEdE(ei, v, df);
    }
    *f = e;
}
#endif
