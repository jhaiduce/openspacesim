#include "Spacecraft.hpp"

#include "SimulationManager.hpp"

Eigen::Vector3d Spacecraft::getAcceleration(SimulationManager*simMgr)
{

  Eigen::Vector3d acceleration(0,0,0);

  // Get the force from each actuator
  for(int i=0;i<actuators.size();i++)
    {
      Eigen::Vector3d actuatorAcceleration=attitude*(actuators[i]->getForce())/mass;
      acceleration+=actuatorAcceleration;
    }

  // Get the gravitational acceleration
  acceleration+=simMgr->gravitationalAcceleration(position,this);

  return acceleration;
}

Eigen::Vector3d Spacecraft::getAngularAcceleration(SimulationManager*simMgr)
{
  Eigen::Vector3d angularAcceleration(0,0,0);

  // Get the torque from each actuator
  for(int i=0;i<actuators.size();i++)
    angularAcceleration+=attitude*(actuators[i]->getTorque());

  return angularAcceleration;
}
