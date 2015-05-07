#ifndef __Actuator_h_
#define __Actuator_h_

#include <Eigen/Core>
#include "SimulationObject.hpp"

class Actuator: public SimulationObject
{
  Eigen::Vector3d Location;
public:
  //! Returns the force applied by the actuator (relative to the spacecraft's origin)
  virtual Eigen::Vector3d getForce()=0;

  //! Returns the torque applied by the actuator (relative to the spacecraft's origin)
  virtual Eigen::Vector3d getTorque()=0;
  virtual ~Actuator(){};
};

class Torquer: public Actuator
{
public:
  //! Normalized axis vector
  Eigen::Vector3d Axis;

  //! Magnitude of torque
  double torque;
  virtual Eigen::Vector3d getForce();
  virtual Eigen::Vector3d getTorque();
};

class Thruster: public Actuator
{
public:
  //! Normalized axis vector
  Eigen::Vector3d Axis;

  //! Magnitude of force
  double force;
  virtual Eigen::Vector3d getForce();
  virtual Eigen::Vector3d getTorque();
};

#endif // #ifndef __Actuator_h_
