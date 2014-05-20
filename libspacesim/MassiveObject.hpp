#ifndef __MassiveObject_h_
#define __MassiveObject_h_

#include "SpaceObject.hpp"

/** MassiveObject is a superclass for large, non-maneuvering objects such as stars, planets, and planetoids. */
class MassiveObject: public SpaceObject
{
  virtual Eigen::Vector3d getAcceleration(SimulationManager*simMgr);
  virtual Eigen::Vector3d getAngularAcceleration(SimulationManager*simMgr);
};

#endif // #ifndef __MassiveObject_h_
