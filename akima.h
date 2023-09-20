#ifndef AKIMA_H
#define AKIMA_H

#define EPS 1e-14
#include <cmath>

int akima_fill(int n, double a, double b, double (*f) (double), double* x, double* f_x);
int akima_approx(int n, double* x, double* f_x, double* c, double d1, double d2, double dn1, double dn);
double akima_Pf(double y, double a, double b, int n, double* x, double* c);



#endif
