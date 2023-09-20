
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

#include "window.h"


#include <cstdio>
#include <cstring>

#include "cheb.h"
#include "akima.h"

int main(int argc, char* argv[]){
  /*
  double a, b;
  int n, k;

  double* x;
  double* f;
  double* alpha;

  if(!(argc == 5 && sscanf(argv[1], "%lf", &a) == 1 && sscanf(argv[2], "%lf", &b) == 1 &&
  sscanf(argv[3], "%d", &n) == 1 && sscanf(argv[4], "%d", &k) == 1)){
    printf("Usage %s a b n k\n", argv[0]);
    return 1;
  }

  x = new double[n];
  f = new double[n];
  alpha = new double[n];
  memset(alpha, 0, n*sizeof(double));
  memset(x, 0, n*sizeof(double));
  memset(f, 0, n*sizeof(double));

  //cheb_fill(n, a, b, k, x, f);
  //cheb_approx(n, x, f, alpha);

  for(int i = 0; i < n; i++){
    printf("%e ", alpha[i]);
  }
  printf("\n^main\n");
*/
  QApplication app (argc, argv);

  QMainWindow *window = new QMainWindow;
  QMenuBar *tool_bar = new QMenuBar (window);
  Window *graph_area = new Window (window);
  QAction *action;

  if (graph_area->parse_command_line (argc, argv))
    {
      QMessageBox::warning (0, "Wrong input arguments!",
                            "Wrong input arguments!");
      return -1;
    }

  action = tool_bar->addAction ("Change function [&0]", graph_area, SLOT (change_func ()));
  action->setShortcut (QString ("0"));

  action = tool_bar->addAction ("Change disp [&1]", graph_area, SLOT (change_disp ()));
  action->setShortcut (QString ("1"));

  action = tool_bar->addAction ("Zoom in [&2]", graph_area, SLOT (zoom_in ()));
  action->setShortcut (QString ("2"));
  action = tool_bar->addAction ("Zoom out [&3]", graph_area, SLOT (zoom_out ()));
  action->setShortcut (QString ("3"));

  action = tool_bar->addAction ("Increase n [&4]", graph_area, SLOT (incr_n ()));
  action->setShortcut (QString ("4"));
  action = tool_bar->addAction ("Decrease n [&5]", graph_area, SLOT (decr_n ()));
  action->setShortcut (QString ("5"));

  action = tool_bar->addAction ("Add error [&6]", graph_area, SLOT (add_error ()));
  action->setShortcut (QString ("6"));
  action = tool_bar->addAction ("Substract error [&7]", graph_area, SLOT (substr_error ()));
  action->setShortcut (QString ("7"));


  action = tool_bar->addAction ("E&xit", window, SLOT (close ()));
  action->setShortcut (QString ("Ctrl+X"));

  tool_bar->setMaximumHeight (30);

  window->setMenuBar (tool_bar);
  window->setCentralWidget (graph_area);
  window->setWindowTitle ("Graph");

  window->show ();
  app.exec ();
  delete tool_bar;
  delete graph_area;
  delete window;
  return 0;
  /*

  delete[] x;
  delete[] f;
  delete[] alpha;
double t;
  printf("Insert number: ");

  while(scanf("%lf", &t)== 1){
    printf("f: %e, Pf: %e\n", func(t, k), cheb_Pf(t, a, b, n, x, alpha));
    printf("Insert number: ");

  }



  return 0;
  */
}
