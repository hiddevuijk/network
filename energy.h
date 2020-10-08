#ifndef GUARD_ENERGY_H
#define GUARD_ENERGY_H

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>

double energy(const gsl_vector *v, void *params)
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

void deriv_energy( const gsl_vector *v, void *params, gsl_vector *df)
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

void energy_deriv_energy( const gsl_vector *x, void *params, double *f, gsl_vector *df)
{

    *f = energy(x, params);
    deriv_energy(x, params, df);
}


#endif
