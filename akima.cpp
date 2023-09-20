#include "akima.h"


double d_f(double* x, double* f_x, int i){
  return (f_x[i+1] - f_x[i])/(x[i+1]-x[i]);
}

double w_j(double* x, double* f_x, int j){
  return fabs(d_f(x, f_x, j) - d_f(x, f_x, j-1));
}

double d_i(double* x, double* f_x, int i, int n, double d1, double d2, double dn1, double dn){
  if(i == 0) return d1;
  if(i == 1) return d2;
  if(i == n-2) return dn1;
  if(i == n-1) return dn;

  double w0, w1;
  w0 = w_j(x, f_x, i-1);
  w1 = w_j(x, f_x, i+1);
  if(fabs(w0*w0 + w1*w1) < EPS){
    return ( (x[i+1]-x[i])*d_f(x, f_x, i-1) + (x[i]-x[i-1])*d_f(x, f_x, i) )/(x[i+1]-x[i-1]);
  }
  return ( w1*d_f(x, f_x, i-1) + w0*d_f(x, f_x, i) )/(w1+w0);

}

int akima_fill(int n, double a, double b, double (*f) (double), double* x, double* f_x){
    for(int i = 0; i < n; i++){
      double xi = a + i*(b-a)/(n-1);
      x[i] = xi;
      f_x[i] = f(xi);
    }
    return 0;
}


int akima_approx(int n, double* x, double* f_x, double* c, double d1, double d2, double dn1, double dn){
  for(int i = 0; i < n-1; i++){
    double di = d_i(x, f_x, i, n, d1, d2, dn1, dn);
    double di1 = d_i(x, f_x, i+1, n, d1, d2, dn1, dn);
    double dfi = d_f(x, f_x, i);
    c[i*4] = f_x[i];
    c[i*4 + 1] = di;
    c[i*4 + 2] = (3*dfi - 2*di - di1)/(x[i+1]-x[i]);
    c[i*4 + 3] = (di + di1 - 2*dfi )/((x[i+1]-x[i])*(x[i+1]-x[i]));

  }
  c[n*4-4] = f_x[n-1];
  c[n*4-3] = 0;
  c[n*4-2] = 0;
  c[n*4-1] = 0;
  return 0;
}



double akima_Pf(double y, double a, double b, int n, double* x, double* c){
  int i = (y-a)*(n-1)/(b-a);
  return c[i*4] + c[i*4 + 1]*(y-x[i]) + c[i*4 + 2] *(y-x[i])*(y-x[i]) + c[i*4+3]*(y-x[i])*(y-x[i])*(y-x[i]);
}
