#include "cheb.h"


double zj(int j, int n){
  return 2*cos(M_PI*(2*j+1)/(2*n));
}

int cheb_fill(int n, double a, double b, double (*f) (double), double* x, double* f_x){
  if(n > 50){
    return 0;
  }
  for(int i = 0; i < n; i++){
    double xi = (a+b)/2 + (b-a)/2 * cos(M_PI*(2*i+1)/(2*n));
    x[i] = xi;
    f_x[i] = f(xi);
  }
  return 0;
}


int cheb_approx(int n, double* /*x*/, double* f, double* a){
  if(n > 50){
    return 0;
  }
  double g0 = 0, g1 = 0, g = 0;
  for(int j = 0; j < n; j++){
    double z = zj(j, n);
    for(int i = 0; i < n; i++){
      if(i == 0){
        g0 = f[j];
        a[i] += g0;

      }
      else if(i == 1){
        g1 = g0 * z / 2;

        a[i] += g1;
      }
      else{
        g = z*g1-g0;
        a[i] += g;

        g0 = g1;
        g1 = g;
      }
    }
  }

  for(int i = 0; i < n; i++){
    if(i == 0){
      a[i] /= n;
    }
    else{
      a[i] *= 2;
      a[i] /= n;
    }
  }



  return 0;
}

double cheb_Pf(double y, double a, double b, int n, double* /*x*/, double* alpha){
  if(n > 50){
    return 0;
  }
  double z = 2*(2*y - (b+a))/(b-a);
  double res = 0;
  double t0 = 1, t1 = z/2, t;
  for(int i = 0; i < n; i++){
    if(i == 0){
      res += alpha[i];
    }
    else if(i == 1){
      res += t1 * alpha[i];
    }
    else{
      t = z*t1 - t0;
      res += t*alpha[i];
      t0 = t1;
      t1 = t;
    }
  }
  return res;
}
