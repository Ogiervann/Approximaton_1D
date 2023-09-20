
#include <QPainter>
#include <stdio.h>

#include "window.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 10
#define L2G(X,Y) (l2g ((X), (Y), min_y, max_y))

static
double f_0 (double /*x*/)
{
  return 1;
}

static
double f_1 (double x)
{
  return x;
}

static
double f_2 (double x)
{
  return x * x;
}

static
double f_3 (double x)
{
  return x*x*x;
}

static
double f_4 (double x)
{
  return x*x*x*x;
}

static
double f_5 (double x)
{
  return exp(x);
}

static
double f_6 (double x)
{
  return 1/(25*x*x+1);
}


static
double df_0 (double /*x*/)
{
  return 0;
}

static
double df_1 (double /*x*/)
{
  return 1;
}

static
double df_2 (double x)
{
  return 2 * x ;
}

static
double df_3 (double x)
{
  return 3*x*x;
}

static
double df_4 (double x)
{
  return 4*x*x*x;
}

static
double df_5 (double x)
{
  return exp(x);
}

static
double df_6 (double x)
{
  return (-50) * x/((25*x*x+1)*(25*x*x+1));
}


Window::Window (QWidget *parent)
  : QWidget (parent)
{
  a = DEFAULT_A;
  b = DEFAULT_B;
  n = DEFAULT_N;
  func_id = 0;
  f_name = "f (x) = 1";
  f = f_0;
}

Window::~Window() {
  delete[] x_cheb;
  delete[] x_akima;
  delete[] f_cheb;
  delete[] f_akima;
  delete[] alpha;
  delete[] c;
}
QSize Window::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize Window::sizeHint () const
{
  return QSize (1000, 1000);
}

int Window::parse_command_line (int argc, char *argv[])
{

  if(!(argc == 5 && sscanf(argv[1], "%lf", &a) == 1 && sscanf(argv[2], "%lf", &b) == 1 && !(b - a < 1.e-6) &&
  sscanf(argv[3], "%d", &n) == 1 && sscanf(argv[4], "%d", &func_id) == 1)){
    printf("Usage %s a b n k\n", argv[0]);
    return -2;
  }
  x_cheb = new double[n];
  x_akima = new double[n];
  f_cheb = new double[n];
  f_akima = new double[n];
  alpha = new double[n];
  c = new double[4*n];
  min_x = a;
  max_x = b;
  func_id--;
  change_func();
/*
  cheb_fill(n, a, b, func_id, x, f_x);
  cheb_approx(n, x, f_x, alpha);
  for(int i = 0; i < n; i++){
    printf("%e ", alpha[i]);
  }
  printf("\n^parse\n");
*/


  return 0;
}

/// change the displayed function
void Window::change_disp()
{
  disp = (disp + 1) % 4;
  update();
}

void Window::zoom_in()
{
  double t = (3*min_x + max_x)/4;
  max_x = (min_x + 3*max_x)/4;
  min_x = t;
  update();
}


void Window::zoom_out()
{
  double t = (3*min_x - max_x)/2;
  max_x = (3*max_x - min_x)/2;
  min_x = t;
  update();
}

void Window::incr_n()
{
  p = 0;
  n *= 2;
  delete[] x_cheb;
  delete[] x_akima;
  delete[] f_cheb;
  delete[] f_akima;
  delete[] alpha;
  delete[] c;
  x_cheb = new double[n];
  x_akima = new double[n];
  f_cheb = new double[n];
  f_akima = new double[n];
  alpha = new double[n];
  c = new double[4*n];


  memset(alpha, 0, n*sizeof(double));
  memset(x_cheb, 0, n*sizeof(double));
  memset(f_cheb, 0, n*sizeof(double));

  cheb_fill(n, a, b, f, x_cheb, f_cheb);
  cheb_approx(n, x_cheb, f_cheb, alpha);

  memset(c, 0, 4*n*sizeof(double));
  memset(x_akima, 0, n*sizeof(double));
  memset(f_akima, 0, n*sizeof(double));


  akima_fill(n, a, b, f, x_akima, f_akima);
  akima_approx(n, x_akima, f_akima, c, df(a), df(x_akima[1]), df(x_akima[n-2]), df(b));

  update();
}

void Window::decr_n()
{
  if(n <= 5) return;
  p = 0;
  n /= 2;
  delete[] x_cheb;
  delete[] x_akima;
  delete[] f_cheb;
  delete[] f_akima;
  delete[] alpha;
  delete[] c;
  x_cheb = new double[n];
  x_akima = new double[n];
  f_cheb = new double[n];
  f_akima = new double[n];
  alpha = new double[n];
  c = new double[4*n];


  memset(alpha, 0, n*sizeof(double));
  memset(x_cheb, 0, n*sizeof(double));
  memset(f_cheb, 0, n*sizeof(double));

  cheb_fill(n, a, b, f, x_cheb, f_cheb);
  cheb_approx(n, x_cheb, f_cheb, alpha);

  memset(c, 0, 4*n*sizeof(double));
  memset(x_akima, 0, n*sizeof(double));
  memset(f_akima, 0, n*sizeof(double));


  akima_fill(n, a, b, f, x_akima, f_akima);
  akima_approx(n, x_akima, f_akima, c, df(a), df(x_akima[1]), df(x_akima[n-2]), df(b));

  update();
}

void Window::add_error()
{
  f_cheb[n/2] += abmax /10;
  f_akima[n/2] += abmax /10;
  p++;

    memset(alpha, 0, n*sizeof(double));
    cheb_approx(n, x_cheb, f_cheb, alpha);
    memset(c, 0, 4*n*sizeof(double));
    akima_approx(n, x_akima, f_akima, c, df(a), df(x_akima[1]), df(x_akima[n-2]), df(b));

  update();

}

void Window::substr_error()
{
  f_cheb[n/2] -= abmax /10;
  f_akima[n/2] -= abmax /10;
  p--;

  memset(alpha, 0, n*sizeof(double));
  cheb_approx(n, x_cheb, f_cheb, alpha);
  memset(c, 0, 4*n*sizeof(double));
  akima_approx(n, x_akima, f_akima, c, df(a), df(x_akima[1]), df(x_akima[n-2]), df(b));

  update();

}



/// change current function for drawing
void Window::change_func ()
{
  func_id = (func_id + 1) % 7;

  switch (func_id)
  {
    case 0:
    f_name = "k=0 f(x)=1";
    f = f_0;
    df = df_0;
    break;
    case 1:
    f_name = "k=1 f(x)=x";
    f = f_1;
    df = df_1;
    break;
    case 2:
    f_name = "k=2 f(x)=x^2";
    f = f_2;
    df = df_2;
    break;
    case 3:
    f_name = "k=3 f(x)=x^3";
    f = f_3;
    df = df_3;
    break;
    case 4:
    f_name = "k=4 f(x)=x^4";
    f = f_4;
    df = df_4;
    break;
    case 5:
    f_name = "k=5 f(x)=exp(x)";
    f = f_5;
    df = df_5;
    break;
    case 6:
    f_name = "k=6 f(x)=1/(25*x^2+1)";
    f = f_6;
    df = df_6;
    break;
  }

  double x1, y1;
  double delta_x = (b - a) / width();
  abmax = 0;
  for (x1 = a; x1 - b < 1.e-6; x1 += delta_x)
    {
      y1 = f (x1);
      if (y1 > abmax)
        abmax = y1;
    }

p = 0;
  memset(alpha, 0, n*sizeof(double));
  memset(f_cheb, 0, n*sizeof(double));

  cheb_fill(n, a, b, f, x_cheb, f_cheb);
  cheb_approx(n, x_cheb, f_cheb, alpha);

  memset(c, 0, 4*n*sizeof(double));
  memset(f_akima, 0, n*sizeof(double));


  akima_fill(n, a, b, f, x_akima, f_akima);
  akima_approx(n, x_akima, f_akima, c, df(a), df(x_akima[1]), df(x_akima[n-2]), df(b));
  /*
  for(int i = 0; i < n; i++){
    printf("%e ", alpha[i]);
  }
  printf("\n");
*/
  update ();


}

QPointF Window::l2g (double x_loc, double y_loc, double y_min, double y_max)
{
  double x_gl = (x_loc - min_x) / (max_x - min_x) * width ();
  double y_gl = (y_max - y_loc) / (y_max - y_min) * height ();
  return QPointF (x_gl, y_gl);
}

/// render graph
void Window::paintEvent (QPaintEvent * /* event */)
{
  QPainter painter (this);
  double x1, x2, y1, y2;
  double max_y, min_y;
  double delta_y, delta_x = (max_x - min_x) / width();
  QPen pen_black(Qt::black, 0, Qt::SolidLine);
  QPen pen_red(Qt::red, 0, Qt::SolidLine);
  QPen pen_blue(Qt::blue, 0, Qt::SolidLine);
  QPen pen_green(Qt::green, 0, Qt::SolidLine);

  painter.setPen (pen_black);
  max_y = min_y = 0;


  // calculate min and max for current function
  for (x1 = min_x; x1 - max_x < 1.e-6; x1 += delta_x)
    {
      if(disp != 3){

      y1 = f (x1);
      if (y1 < min_y)
        min_y = y1;
      if (y1 > max_y)
        max_y = y1;
      }
      if(disp == 0 || disp == 2){

      y1 = cheb_Pf (x1, a, b, n, x_cheb, alpha);
      if (y1 < min_y)
        min_y = y1;
      if (y1 > max_y)
        max_y = y1;
      }
      if(disp == 1 || disp == 2){

      y1 = akima_Pf (x1, a, b, n, x_akima, c);
      if (y1 < min_y)
        min_y = y1;
      if (y1 > max_y)
        max_y = y1;
      }
      if(disp == 3){
        y1 = f(x1) - akima_Pf (x1, a, b, n, x_akima, c);
        if (y1 < min_y)
        min_y = y1;
        if (y1 > max_y)
        max_y = y1;
        if(n <= 50){
          y1 = f(x1) - cheb_Pf (x1, a, b, n, x_cheb, alpha);
          if (y1 < min_y)
            min_y = y1;
          if (y1 > max_y)
            max_y = y1;
          }
        }

    }
    double fmm = (fabs(max_y) > fabs(min_y)? fabs(max_y): fabs(min_y));
    printf("max{|F_min|, |F_max|} = %e\n", fmm);

    sprintf(info, "p = %d; n = %d; max{|F_min|, |F_max|} = %e", p, n, fmm);
  delta_y = 0.01 * (max_y - min_y);
  min_y -= delta_y;
  max_y += delta_y;


  if(disp != 3){

  // draw approximated line for graph
  x1 = min_x;
  y1 = f (x1);
  for (x2 = x1 + delta_x; x2 - max_x < 1.e-6; x2 += delta_x)
    {
      y2 = f (x2);
      // local coords are converted to draw coords
      painter.drawLine (L2G(x1, y1), L2G(x2, y2));

      x1 = x2, y1 = y2;
    }
  x2 = max_x;
  y2 = f (x2);
  painter.drawLine (L2G(x1, y1), L2G(x2, y2));
  }

  if(disp == 0 || disp == 2){

  painter.setPen(pen_blue);
  // draw approximated line for graph
  x1 = min_x;
  y1 = cheb_Pf (x1, a, b, n, x_cheb, alpha);
  for (x2 = x1 + delta_x; x2 - max_x < 1.e-6; x2 += delta_x)
    {
      y2 = cheb_Pf (x2, a, b, n, x_cheb, alpha);
      // local coords are converted to draw coords
      painter.drawLine (L2G(x1, y1), L2G(x2, y2));

      x1 = x2, y1 = y2;
    }
  x2 = max_x;
  y2 = cheb_Pf (x2, a, b, n, x_cheb, alpha);
  painter.drawLine (L2G(x1, y1), L2G(x2, y2));
  }

if(disp == 1 || disp == 2){

  painter.setPen(pen_green);
  // draw approximated line for graph
  x1 = min_x;
  y1 = akima_Pf (x1, a, b, n, x_akima, c);
  for (x2 = x1 + delta_x; x2 - max_x < 1.e-6; x2 += delta_x)
    {
      y2 = akima_Pf (x2, a, b, n, x_akima, c);
      // local coords are converted to draw coords
      painter.drawLine (L2G(x1, y1), L2G(x2, y2));

      x1 = x2, y1 = y2;
    }
  x2 = max_x;
  y2 = akima_Pf (x2, a, b, n, x_akima, c);
  painter.drawLine (L2G(x1, y1), L2G(x2, y2));
}

if(disp == 3){
  painter.setPen(pen_blue);
  // draw approximated line for graph
  if(n <= 50){

  x1 = min_x;
  y1 = f(x1) - cheb_Pf (x1, a, b, n, x_cheb, alpha);
  for (x2 = x1 + delta_x; x2 - max_x < 1.e-6; x2 += delta_x)
    {
      y2 = f(x2) - cheb_Pf (x2, a, b, n, x_cheb, alpha);
      // local coords are converted to draw coords
      painter.drawLine (L2G(x1, y1), L2G(x2, y2));

      x1 = x2, y1 = y2;
    }
  x2 = max_x;
  y2 = f(x2) - cheb_Pf (x2, a, b, n, x_cheb, alpha);
  painter.drawLine (L2G(x1, y1), L2G(x2, y2));
}

    painter.setPen(pen_green);
    // draw approximated line for graph
    x1 = min_x;
    y1 = f(x1) - akima_Pf (x1, a, b, n, x_akima, c);
    for (x2 = x1 + delta_x; x2 - max_x < 1.e-6; x2 += delta_x)
      {
        y2 = f(x2) - akima_Pf (x2, a, b, n, x_akima, c);
        // local coords are converted to draw coords
        painter.drawLine (L2G(x1, y1), L2G(x2, y2));

        x1 = x2, y1 = y2;
      }
    x2 = max_x;
    y2 = f(x2) - akima_Pf (x2, a, b, n, x_akima, c);
    painter.drawLine (L2G(x1, y1), L2G(x2, y2));
}

  // draw axis
  painter.setPen (pen_red);
  painter.drawLine (L2G(a, 0), L2G(b, 0));
  painter.drawLine (L2G(0, min_y), L2G(0, max_y));

  // render function name
  painter.setPen ("blue");
  painter.drawText (0, 20, f_name);
  painter.drawText (0, 40, info);

}
