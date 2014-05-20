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
