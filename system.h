#ifndef GUARD_SYSTEM_H
#define GUARD_SYSTEM_H

#include "network.h"
#include "system.h"

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>

#include <vector>

#include <iostream>

class NetworkMinimizer
{
  public:
    NetworkMinimizer(int N, const std::vector<double>& rInit, Network *network);
    ~NetworkMinimizer();

    int N;
    Network *network;


    gsl_vector *r;
    gsl_multimin_function_fdf multimin_func;

    const gsl_multimin_fdfminimizer_type *T; 
    gsl_multimin_fdfminimizer *s;

    std::vector<double> get_positions();

    // affine transformation
    void minimize(int maxIter = 1000);
    void minimize_affine(double Ddelta);
};


NetworkMinimizer::NetworkMinimizer( int N,const std::vector<double>& rInit, Network *network)
: N(N)
{


    // initialize position vector
    r = gsl_vector_alloc(2*N);
    for(int i=0;i<2*N; ++i) gsl_vector_set(r,i,rInit[i]);

    // set multimin_func
    multimin_func.n = 2*N;
    multimin_func.f = &energy;
    multimin_func.df = &deriv_energy;
    multimin_func.fdf = &energy_deriv_energy;
    multimin_func.params = (void *) network;

    // initialize minimizer type
    //T = gsl_multimin_fdfminimizer_conjugate_fr;
    //T = gsl_multimin_fdfminimizer_conjugate_pr;
    //T = gsl_multimin_fdfminimizer_vector_bfgs2; 
    //T = gsl_multimin_fdfminimizer_vector_bfgs; 
    T = gsl_multimin_fdfminimizer_steepest_descent; 

    // initialize minimizer s
    s = gsl_multimin_fdfminimizer_alloc(T,2*N);
    gsl_multimin_fdfminimizer_set(s, &multimin_func, r, 1., .01);

}

NetworkMinimizer::~NetworkMinimizer()
{
    gsl_multimin_fdfminimizer_free(s);
    gsl_vector_free(r);
}

void NetworkMinimizer::minimize(int maxIter)
{
    gsl_multimin_fdfminimizer_restart(s);
    int iter = 0;
    int status;
    do {
        
        iter++;
        status = gsl_multimin_fdfminimizer_iterate(s);
        std::cout <<"iter: " <<  iter << "\n"; 
        if(status) break;

        status = gsl_multimin_test_gradient(s->gradient, 1.e-9);

        if(status == GSL_SUCCESS ) std::cout << "min found at: \n";

        std::cout << gsl_vector_get(s->x, 0) << "\t";
        std::cout << gsl_vector_get(s->x, 2) << "\t";
        std::cout << gsl_vector_get(s->x, 4) << "\n";
        std::cout << gsl_vector_get(s->x, 1) << "\t";
        std::cout << gsl_vector_get(s->x, 3) << "\t";
        std::cout << gsl_vector_get(s->x, 5) << "\n";
        std::cout << s->f << "\n";
        std::cout << "-----------------\n";



    } while( status == GSL_CONTINUE && iter < maxIter );


}

void NetworkMinimizer::minimize_affine(double Ddelta)
{
    for(int i=0;i<2*N;i+=2) 
        (s->x->data)[i] += Ddelta * (s->x->data)[i+1];

    minimize();

}

std::vector<double> NetworkMinimizer::get_positions()
{
    std::vector<double> pos(2*N);
    for(int i=0;i<2*N;++i) pos[i] = gsl_vector_get(s->x,i);
    return pos;
}

#endif
