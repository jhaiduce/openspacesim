#ifndef __Spacecraft_h_
#define __Spacecraft_h_

#include "SpaceObject.hpp"
#include "Sensor.hpp"
#include "Actuator.hpp"
#include <vector>

class Spacecraft: public SpaceObject
{
public:
  std::vector<Sensor*> sensors;
  std::vector<Actuator*> actuators;
  virtual Eigen::Vector3d getAcceleration(SimulationManager*simMgr);
  virtual Eigen::Vector3d getAngularAcceleration(SimulationManager*simMgr);
  virtual ~Spacecraft() {};
};

#endif // #ifndef __Spacecraft_h_
