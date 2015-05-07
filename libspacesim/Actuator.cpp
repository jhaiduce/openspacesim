#include "Actuator.hpp"

Eigen::Vector3d Torquer::getForce()
{
  Eigen::Vector3d force(0,0,0);
  return force;
}

Eigen::Vector3d Torquer::getTorque()
{
  // For a simple torquer, just return the axis vector multiplied by the magnitude
  return Axis*torque;
}

Eigen::Vector3d Thruster::getForce()
{
  // For a simple thruster, just return the axis vector multiplied by the magnitude
  return Axis*force;
}

Eigen::Vector3d Thruster::getTorque()
{
  Eigen::Vector3d torque(0,0,0);
  return torque;
}
