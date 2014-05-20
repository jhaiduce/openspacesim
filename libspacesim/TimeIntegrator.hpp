#ifndef __TimeIntegrator_h_
#define __TimeIntegrator_h_

template <class T>
class dStateFunctor
{
 public:
  virtual T operator()(double t, T y)=0;
};

template <class T>
class TimeIntegrator
{
 public:
  double dt;
  TimeIntegrator(double dt): dt(dt){};
  virtual T step(dStateFunctor<T> &f, T y0, double t0, double & t1, double & dt)=0;
  T step(dStateFunctor<T> &f, T y0, double t0, double & t1);
  virtual T run(dStateFunctor<T> &f, T y0, double t0, double t1)=0;
};

/** 4th order Runge-Kutta integrator */
template <class T>
class RK4Integrator: public TimeIntegrator<T>
{
 public:
  RK4Integrator(double dt): TimeIntegrator<T>(dt){};
  T step(dStateFunctor<T> &f, T y0, double t0, double & t1, double & dt);
  T run(dStateFunctor<T> &f, T y0, double t0, double t1);
};

/** Runge-Kutta Fehlberg integrator */
template <class T>
class RKFIntegrator: public TimeIntegrator<T>
{
 public:
  double maxerr;
  double dtmin;
  double dtmax;
  double dtinit;
  RKFIntegrator (double dtinit, double dtmin, double dtmax, double maxerr):
  dtinit(dtinit), dtmin(dtmin), dtmax(dtmax), maxerr(maxerr), TimeIntegrator<T>(dtinit) { };
  T step(dStateFunctor<T> &f, T y0, double t0, double & t1, double & dt);
  T step(dStateFunctor<T> &f, T y0, double t0, double & t1);
  T run(dStateFunctor<T> &f, T y0, double t0, double t1);
};

#endif // #ifndef __TimeIntegrator_h_
