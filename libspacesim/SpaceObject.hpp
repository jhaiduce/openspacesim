#ifndef __SpaceObject_h_
#define __SpaceObject_h_

#include <Eigen/Core>
#include <Eigen/Geometry>
#include "SimulationObject.hpp"

class SimulationManager;

/** Base class for all objects participating in the simulation. */
class SpaceObject: public SimulationObject
{
 public:
  double mass;
  Eigen::Vector3d position;
  Eigen::Vector3d velocity;
  Eigen::Quaterniond attitude;
  Eigen::Vector3d angularVelocity;
  Eigen::Vector3d cg;
  Eigen::Matrix3d moi;
  virtual Eigen::Vector3d getAcceleration(SimulationManager*simMgr)=0;
  virtual Eigen::Vector3d getAngularAcceleration(SimulationManager*simMgr)=0;
  virtual ~SpaceObject(){};
};

#endif // #ifndef __SpaceObject_h_
