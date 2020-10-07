
#include "network.h"

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>

#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

double my_f(const gsl_vector *v, void *params)
{
    double x1,y1,x2,y2;
    Network *net = (Network *) params;

    double l;
    double E = 0;
    for(int si=0; si<net->Nspring; ++si) {
        int node1 = net->springs[si].I1;
        int node2 = net->springs[si].I2;
        double l0 = net->springs[si].l0;
        x1 = gsl_vector_get(v, 2*node1);
        y1 = gsl_vector_get(v, 2*node1+1);
        x2 = gsl_vector_get(v, 2*node2);
        y2 = gsl_vector_get(v, 2*node2+1);

        double dx = x1-x2;
        double dy = y1-y2;
        l = sqrt( dx*dx + dy*dy);
        E += (l0-l)*(l0-l)/2.;
    }


    return E;
}

void my_df( const gsl_vector *v, void *params, gsl_vector *df)
{
    double x1,y1,x2,y2;
    Network *net = (Network *) params;
    for(long unsigned int i=0; i<df->size; ++i) (df->data)[i] = 0;
    double l;
    double Fx, Fy;
    for(int si=0; si<net->Nspring; ++si) {
        int node1 = net->springs[si].I1;
        int node2 = net->springs[si].I2;
        double l0 = net->springs[si].l0;
        x1 = gsl_vector_get(v, 2*node1);
        y1 = gsl_vector_get(v, 2*node1+1);
        x2 = gsl_vector_get(v, 2*node2);
        y2 = gsl_vector_get(v, 2*node2+1);

        double dx = x2-x1;
        double dy = y2-y1;
        l = sqrt( dx*dx + dy*dy);
        Fx = (l0-l)*dx/l;
        Fy = (l0-l)*dy/l;
        (df->data)[2*node1]   += Fx;
        (df->data)[2*node2]   -= Fx;
        (df->data)[2*node1+1] += Fy;
        (df->data)[2*node2+1] -= Fy;
    }

}

void my_fdf( const gsl_vector *x, void *params, double *f, gsl_vector *df)
{

    *f = my_f(x, params);
    my_df(x, params, df);
}

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
    my_func.f = &my_f;
    my_func.df = &my_df;
    my_func.fdf = &my_fdf;
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

