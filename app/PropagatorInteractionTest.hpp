#ifndef __PropagatorInteractionTest_h_
#define __PropagatorInteractionTest_h_

#include "BaseApplication.hpp"
#include <vector>
#include <OgreTimer.h>
#include "SimulationManager.hpp"
#include "MassiveObject.hpp"
#include "Spacecraft.hpp"

class PropagatorTestApp : public BaseApplication
{
public:
  PropagatorTestApp(void);
  virtual ~PropagatorTestApp(void);
  double speedup;
  double lastFrameTime;
  Ogre::Timer* timer;
  SimulationManager simMgr;
  MassiveObject earthObj;
  Spacecraft scObj;
  Torquer torqueX, torqueY, torqueZ;
  std::vector<Torquer*> torquers;
  Ogre::SceneNode*earthNode;
protected:
  virtual void createScene(void);
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

  // OIS::KeyListener
  virtual bool keyPressed( const OIS::KeyEvent &arg );
  virtual bool keyReleased( const OIS::KeyEvent &arg );
  virtual bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
  virtual bool buttonReleased( const OIS::JoyStickEvent &arg, int button );

  // OIS::JoyStickListener
  virtual bool axisMoved( const OIS::JoyStickEvent &arg, int axis );
  void updateCameraState();
  void updateEarthState();
};

#endif // #ifndef __PropagatorTestApp_h_
