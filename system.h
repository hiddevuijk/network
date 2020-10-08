#ifndef GUARD_SYSTEM_H
#define GUARD_SYSTEM_H

#include "network.h"
#include "system.h"

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>

#include <vector>

class System
{
  public:
    System(int N , std::vector<double>rInit);

    Network network;
    //Graph graph
    //xInit;

    // shear
    double delta;

    void shear(double ddelta);

    gsl_vector *r;
    gsl_multimin_function_fdf multimin_func;

    const gsl_multimin_fdfminimizer_type *T; 
    gsl_multimin_fdfminimizer *s;
};


System::System( int N, std::vector<double> rInit)
{

    // initialize position vector
    r = gsl_vector_alloc(2*N);
    for(int i=0;i<2*N; ++i) gsl_vector_set(r,i,rInit[i]);

    // set multimin_func
    multimin_func.n = 2*N;
    multimin_func.f = &energy;
    multimin_func.df = &deriv_energy;
    multimin_func.fdf = &energy_deriv_energy;
    multimin_func.params = (void *) &(this->network);

    // initialize minimizer type
    //T = gsl_multimin_fdfminimizer_conjugate_fr;
    //T = gsl_multimin_fdfminimizer_conjugate_pr;
    T = gsl_multimin_fdfminimizer_vector_bfgs2; 
    //T = gsl_multimin_fdfminimizer_vector_bfgs; 
    //T = gsl_multimin_fdfminimizer_steepest_descent; 

    // initialize minimizer s
    s = gsl_multimin_fdfminimizer_alloc(T,6);
    gsl_multimin_fdfminimizer_set(s, &multimin_func, r, 1., .1);

}

#endif
