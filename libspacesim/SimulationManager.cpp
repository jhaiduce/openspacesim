#include "SimulationManager.hpp"
#include "TimeIntegrator.hpp"
#include <iostream>

/** Updates the state vector from the attributes of the objects in the objects vector */
void SimulationManager::updateState()
{
  for(int i=0; i<objects.size(); i++)
    {
      state.segment(i*13,3)=objects[i]->position;
      state.segment(i*13+3,3)=objects[i]->velocity;
      state.segment(i*13+6,4)=objects[i]->attitude.coeffs();
      state.segment(i*13+10,3)=objects[i]->angularVelocity;
    }
}

/** Updates the objects in the objects vector with the data in the state vector */
void SimulationManager::updateObjectsWithState(Eigen::VectorXd state)
{
  for(int i=0; i<objects.size(); i++)
    {
      objects[i]->position=state.segment(i*13,3);
      objects[i]->velocity=state.segment(i*13+3,3);
      objects[i]->attitude.coeffs()=state.segment(i*13+6,4);
      objects[i]->angularVelocity=state.segment(i*13+10,3);
    }
}

/** Gets the derivative of the state vector */
Eigen::VectorXd SimulationManager::getStateDerivative(Eigen::VectorXd state)
{
  Eigen::VectorXd dstate(state.size());

  // Update the objects with the state vector
  updateObjectsWithState(state);

  // Iterate through all the objects and get the derivatives of their respective portions of the state vector
  for(int i=0; i<objects.size(); i++)
    {
      dstate.segment(i*13,3)=objects[i]->velocity;
      dstate.segment(i*13+3,3)=objects[i]->getAcceleration(this);
      Eigen::Quaterniond omega;
      omega.coeffs()<<objects[i]->angularVelocity,0;
      dstate.segment(i*13+6,4)=(omega*objects[i]->attitude).coeffs()*0.5;
      dstate.segment(i*13+10,3)=objects[i]->getAngularAcceleration(this);
    }

  return dstate;
}

/** Calculates the global gravitational acceleration at the given location, excluding the given object */
Eigen::Vector3d SimulationManager::gravitationalAcceleration(Eigen::Vector3d pos, SpaceObject*obj)
{
  double G=6.67384e-11;
  Eigen::Vector3d accel(0,0,0);
  for(int i=0; i<objects.size(); i++)
    {
      if(objects[i]==obj) continue;
      Eigen::Vector3d p1=state.segment(i*13,3);
      Eigen::Vector3d relpos=p1-pos;
      double r=relpos.norm();
      if(r>0) accel+=relpos*(G*objects[i]->mass/pow(r,3));
    }
  return accel;
}

/** Add an object to the simulation */
void SimulationManager::addObject(SpaceObject &object)
{
  objects.push_back(&object);
  state.resize(objects.size()*13);
}

/** Functor which calls getStateDerivative */
template<class T>
class dGlobalStateFunctor: public dStateFunctor<T>
{
  SimulationManager*simMgr;
public:
  dGlobalStateFunctor(SimulationManager*simMgr):simMgr(simMgr){};
  T operator()(double t, T y){return simMgr->getStateDerivative(y);};
};

/** Integrate through the given time period with the time step dt (dt may be treated as a default time step and may not be the actual time step used) */
void SimulationManager::run(double t0,double t1,double dt)
{
  updateState();
  dGlobalStateFunctor<Eigen::VectorXd> functor=dGlobalStateFunctor<Eigen::VectorXd>(this);
  RK4Integrator<Eigen::VectorXd> integrator(dt);
  state=integrator.step(functor,state,t0,t1,dt);
  updateObjectsWithState(state);
}
