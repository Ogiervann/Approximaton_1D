#ifndef CHEB_H
#define CHEB_H


#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <cstdio>


int cheb_fill(int n, double a, double b, double (*f) (double), double* x, double* f_x);
int cheb_approx(int n, double* x, double* f, double* a);
double cheb_Pf(double y, double a, double b, int n, double* x, double* alpha);

#endif
