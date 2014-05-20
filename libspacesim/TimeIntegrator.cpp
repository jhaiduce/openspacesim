#include "TimeIntegrator.hpp"

#include <cmath>
#include <Eigen/Core>

/** Integrate a single step using the default time step */
template<class T>
T TimeIntegrator<T>::step(dStateFunctor<T> &f, T y, double t0, double & t1)
{
  return step(f,y,t0,t1,dt);
}

/** Integrate a single step using the given time step */
template<class T>
T RK4Integrator<T>::step(dStateFunctor<T> &f, T y, double t0, double & t1, double & dt)
{
  T k1=f(t0,y);
  T k2=f(t0+dt/2,y+k1*dt/2);
  T k3=f(t0+dt/2,y+k2*dt/2);
  T k4=f(t0+dt,y+k3*dt);
  t1=t0+dt;
  return y+(k1+k2*2+k3*2+k4)*dt/6;
}

/** Integrate to the requested time */
template<class T>
T RK4Integrator<T>::run(dStateFunctor<T> &f, T y, double t0, double t1)
{
  double t=t0;

  double dt=this->dt;

  double tn;

  for(int i=0; i<(t1-t0)/dt; i++);
  {
    tn=t+dt;
    y=TimeIntegrator<T>::step(f,y,t,tn);
  }
  y=step(f,y,t,t,t1);
  return y;
}

/** Integrate a single step using the given time step */
template<class T>
T RKFIntegrator<T>::step(dStateFunctor<T> &f, T y, double t0, double & t1, double & dt)
{

  double w=0.5;
  while(1)
    {
      T k1=dt*f(t0,y);
      T k2=dt*f(t0+dt/4,y+k1/4);
      T k3=dt*f(t0+3*dt/8,y+3*k1/32+9*k2/32);
      T k4=dt*f(t0+12*dt/13, y+1932*k1/2197-7200*k2/2197+7296*k3/2197);
      T k5=dt*f(t0+dt, y+439*k1/216-8*k2+3680*k3/513-845*k4/4104);
      T k6=dt*f(t0+dt/2, y-8*k1/27+2*k2-3544*k3/2565+1859*k4/4104-11*k5/40);
      T y1=w+25*k1/216+1408*k3/2565+2197*k4/4104-k5/5;
      T y2=w+16*k1/135+6656*k3/12825+28561*k4/56430-9*k5/50+2*k6/55;
      double R=abs(y1-y2)/dt;
      double delta=0.84*std::pow(maxerr/R,0.25);
      if(R<=maxerr)
	{
	  t1=t0+dt;
	  dt*=delta;
	  return y1;
	}
    }
}

/** Integrate to the requested time */
template<class T>
T RKFIntegrator<T>::run(dStateFunctor<T> &f, T y, double t0, double t1)
{
  double t=t0;

  double dt=dtinit;

  while(t+dt<t1)
  {
    y=step(f,y,t,t,dt);
    if(t+dt>t1) dt=t1-t;
  }
  return y;
}

// Instantiate template implementations
template class RK4Integrator<Eigen::VectorXd>;
