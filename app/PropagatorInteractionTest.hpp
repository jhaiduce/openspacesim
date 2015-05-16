#ifndef __PropagatorInteractionTest_h_
#define __PropagatorInteractionTest_h_

#include "BaseApplication.hpp"
#include <vector>
#include <OgreTimer.h>
#include "SimulationManager.hpp"
#include "MassiveObject.hpp"
#include "Spacecraft.hpp"
#include "GameManager.hpp"

class PropagatorTestApp : public BaseApplication
{
public:
  PropagatorTestApp(void);
  virtual ~PropagatorTestApp(void);
  double speedup;
  double lastFrameTime;
  Ogre::Timer* timer;
  GameManager* gameMgr;
  MassiveObject* earthObj;
  Spacecraft* scObj;
  Ogre::Node* earthNode;
  Ogre::Node* stationNode;
protected:
  virtual void createScene(void);
  virtual bool setup();
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

  // OIS::KeyListener
  virtual bool keyPressed( const OIS::KeyEvent &arg );
  virtual bool keyReleased( const OIS::KeyEvent &arg );
  virtual bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
  virtual bool buttonReleased( const OIS::JoyStickEvent &arg, int button );

  // OIS::JoyStickListener
  virtual bool axisMoved( const OIS::JoyStickEvent &arg, int axis );
};

#endif // #ifndef __PropagatorTestApp_h_
