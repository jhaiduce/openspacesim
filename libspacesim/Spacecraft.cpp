#include "Spacecraft.hpp"

#include "SimulationManager.hpp"

Eigen::Vector3d Spacecraft::getAcceleration(SimulationManager*simMgr)
{
  // Get and return the gravitational acceleration
  return simMgr->gravitationalAcceleration(position,this);
}

Eigen::Vector3d Spacecraft::getAngularAcceleration(SimulationManager*simMgr)
{
  Eigen::Vector3d angularAcceleration(0,0,0);

  // Get the torque from each actuator
  for(int i=0;i<actuators.size();i++)
    angularAcceleration+=actuators[i]->getTorque();

  return angularAcceleration;
}
