#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets/QtWidgets>
#include "cheb.h"
#include "akima.h"

class Window : public QWidget
{
  Q_OBJECT

private:
  int func_id = 0;
  int disp = 0;
  const char *f_name = nullptr;
  char info[100];
  double a = -10;
  double min_x = -10;
  double b = 10;
  double max_x = 10;
  int n = 10;
  double (*f) (double) = nullptr;
  double (*df) (double) = nullptr;
  double* x_cheb = nullptr;
  double* x_akima = nullptr;
  double* alpha = nullptr;
  double* c = nullptr;
  double *f_cheb = nullptr;
  double *f_akima = nullptr;
  double abmax = 0;
  int p = 0;


public:
  Window (QWidget *parent);
  ~Window();

  QSize minimumSizeHint () const;
  QSize sizeHint () const;

  int parse_command_line (int argc, char *argv[]);
  QPointF l2g (double x_loc, double y_loc, double y_min, double y_max);
public slots:
  void change_func ();
  void change_disp ();

  void zoom_in();
  void zoom_out();

  void incr_n();
  void decr_n();

  void add_error();
  void substr_error();

protected:
  void paintEvent (QPaintEvent *event);
};

#endif
