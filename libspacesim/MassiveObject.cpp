#include "MassiveObject.hpp"

#include "SimulationManager.hpp"

Eigen::Vector3d MassiveObject::getAcceleration(SimulationManager*simMgr)
{
  return simMgr->gravitationalAcceleration(position,this);
}

Eigen::Vector3d MassiveObject::getAngularAcceleration(SimulationManager*simMgr)
{
  Eigen::Vector3d angularAcceleration(0,0,0);
  return angularAcceleration;
}
