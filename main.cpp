
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

    int Nspring = 3;
    int Nbend = 0;

    Network network(Nspring, Nbend);
    network.springs[0].I1 = 0;
    network.springs[0].I2 = 1;
    network.springs[0].l0 = 2.;
    
    network.springs[1].I1 = 0;
    network.springs[1].I2 = 2;
    network.springs[1].l0 = 2.;

    network.springs[2].I1 = 1;
    network.springs[2].I2 = 2;
    network.springs[2].l0 = 1.;



    gsl_multimin_function_fdf my_func;
    my_func.n = 6;
    my_func.f = &energy;
    my_func.df = &deriv_energy;
    my_func.fdf = &energy_deriv_energy;
    my_func.params = (void *) &network;


    const gsl_multimin_fdfminimizer_type *T;  
    //T = gsl_multimin_fdfminimizer_conjugate_fr;
    //T = gsl_multimin_fdfminimizer_conjugate_pr;
    T = gsl_multimin_fdfminimizer_vector_bfgs2; 
    //T = gsl_multimin_fdfminimizer_vector_bfgs; 
    //T = gsl_multimin_fdfminimizer_steepest_descent; 

    gsl_multimin_fdfminimizer *s;
    s = gsl_multimin_fdfminimizer_alloc(T,6);


    gsl_vector *r;
    r = gsl_vector_alloc(6);
    gsl_vector_set(r, 0, 0.0); // x0
    gsl_vector_set(r, 1, 0.0); // y0
    gsl_vector_set(r, 2, 1.0); // x1
    gsl_vector_set(r, 3, 0.0); // y1
    gsl_vector_set(r, 4, 1.0); // x1
    gsl_vector_set(r, 5, 4.0); // y1

    gsl_multimin_fdfminimizer_set(s, &my_func, r, 1., .1);
    gsl_multimin_fdfminimizer_restart(s);

    cout << gsl_vector_get(s->x, 0) << "\t";
    cout << gsl_vector_get(s->x, 2) << "\n";
    cout << gsl_vector_get(s->x, 4) << "\n";
    cout << gsl_vector_get(s->x, 1) << "\t";
    cout << gsl_vector_get(s->x, 3) << "\t";
    cout << gsl_vector_get(s->x, 5) << "\n";
    cout << s->f << "\n";
    cout << "-----------------\n";

    int iter = 0;
    int status;
    do {
        
        iter++;
        status = gsl_multimin_fdfminimizer_iterate(s);
        cout <<"iter: " <<  iter << "\n"; 
        if(status) break;

        status = gsl_multimin_test_gradient(s->gradient, 1.e-12);

        if(status == GSL_SUCCESS ) cout << "min found at: \n";

        cout << gsl_vector_get(s->x, 0) << "\t";
        cout << gsl_vector_get(s->x, 2) << "\t";
        cout << gsl_vector_get(s->x, 4) << "\n";
        cout << gsl_vector_get(s->x, 1) << "\t";
        cout << gsl_vector_get(s->x, 3) << "\t";
        cout << gsl_vector_get(s->x, 5) << "\n";
        cout << s->f << "\n";
        cout << "-----------------\n";



    } while( status == GSL_CONTINUE && iter < 10000 );




    gsl_multimin_fdfminimizer_free(s);
    gsl_vector_free(r);


    return 0;
}

