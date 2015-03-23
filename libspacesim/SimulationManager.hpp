#ifndef __SimulationManager_h_
#define __SimulationManager_h_

#include "SpaceObject.hpp"
#include <vector>

/** SimulationManager knows about all the objects in the simulation, and handles the process of getting the acceleration calculations from each object, time-stepping through the simulation, and updating objects with the new state calculated through time iteration. */
class SimulationManager
{
  //! State vector
  Eigen::VectorXd state;

  //! Vector of all the objects in the simulation
  std::vector<SpaceObject*> objects;

  void updateState();
  void updateObjectsWithState(Eigen::VectorXd state);

public:

  Eigen::VectorXd getStateDerivative(Eigen::VectorXd state);
  void run(double t0, double t1, double dt);
  Eigen::Vector3d gravitationalAcceleration(Eigen::Vector3d pos, SpaceObject*obj);
  void addObject(SpaceObject *object);
  SpaceObject* getObject(int i);
  void removeAllObjects();
};

#endif // #ifndef __SimulationManager_h_
