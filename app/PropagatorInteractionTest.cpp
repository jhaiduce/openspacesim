#include "PropagatorInteractionTest.hpp"

//-------------------------------------------------------------------------------------
PropagatorTestApp::PropagatorTestApp(void)
{
}
//-------------------------------------------------------------------------------------
PropagatorTestApp::~PropagatorTestApp(void)
{
}

//-------------------------------------------------------------------------------------
void PropagatorTestApp::createScene(void)
{

  // Set the scene's ambient light
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

  // Setup material manager
  Ogre::MaterialManager & materialManager = Ogre::MaterialManager::getSingleton();
  Ogre::String resourceGroupName = "PlanetTextures";
  Ogre::ResourceGroupManager & rgMgr = Ogre::ResourceGroupManager::getSingleton();
  rgMgr.createResourceGroup(resourceGroupName);
  Ogre::String texDirPath = "materials";
  bool isRecursive = false;
  rgMgr.addResourceLocation(texDirPath, "FileSystem", resourceGroupName, isRecursive);
  rgMgr.initialiseResourceGroup(resourceGroupName);
  rgMgr.loadResourceGroup(resourceGroupName);
  Ogre::MaterialPtr material = materialManager.create("EarthSimple",resourceGroupName);

  // Create SkyBox
  mSceneMgr->setSkyBox(true,"SkyBox");

  // Create the earth
  Ogre::Entity* earthEnt = mSceneMgr->createEntity("mySphere", Ogre::SceneManager::PT_SPHERE);
  earthEnt->setMaterialName("EarthSimple");

  earthObj.mass=5.972e24;
  earthObj.position<<0,0,0;
  earthObj.velocity<<0,0,0;
  earthObj.attitude=Eigen::Quaterniond(0,0,0,1);
  earthObj.angularVelocity<<0,-1.745e-4,0;
  simMgr.addObject(earthObj); 

  // Create a SceneNode and attach the Entity to it
  earthNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("EarthNode");
  earthNode->attachObject(earthEnt); 
  earthNode->scale( 6378100./50,6356800./50,6378100./50 ); 

  // Set the spacecraft's initial state
  scObj.mass=1;
  scObj.position<<-16000000,0,0;
  scObj.velocity<<0,-67000,0;
  scObj.attitude=Eigen::Quaterniond(sqrt(0.5),0,-sqrt(0.5),0);
  scObj.angularVelocity<<0,0,0;

  // Put the torquers in the torquers vector
  // (order here is important; note that Y and Z are reversed)
  torquers.push_back(&torqueX);
  torquers.push_back(&torqueZ);
  torquers.push_back(&torqueY);

  // Set the initial state of the torquers
  torqueX.Axis<<1,0,0;
  torqueY.Axis<<0,1,0;
  torqueZ.Axis<<0,0,1;
  for(int i=0;i<3;i++)
    torquers[i]->torque=0;

  // Add the actuators to the spacecraft
  scObj.actuators.push_back(&torqueX);
  scObj.actuators.push_back(&torqueY);
  scObj.actuators.push_back(&torqueZ);

  // Add the spacecraft to the simulation
  simMgr.addObject(scObj);

  // Move camera to spacecraft location
  updateCameraState();

  mCamera->setFarClipDistance(100000000);

  // Create a Light and set its position
  Ogre::Light* light = mSceneMgr->createLight("MainLight");
  light->setPosition(2000000.0f, 8000000.0f, 5000000.0f);

  // Initialize the timer
  timer=new Ogre::Timer();
  timer->reset();
  lastFrameTime=0;
}

void PropagatorTestApp::updateCameraState()
{

  // Move camera to spacecraft location
  Eigen::Vector3f positionVec=scObj.position.cast<float>();
  mCamera->setPosition(Ogre::Vector3( static_cast<Ogre::Real*>(positionVec.data()) ));
  Eigen::Vector4f attitudeVec=scObj.attitude.coeffs().cast<float>();
  mCamera->setOrientation(Ogre::Quaternion(static_cast<Ogre::Real*>(attitudeVec.data()) ));

}

void PropagatorTestApp::updateEarthState()
{

  // Move Earth to the location calculated by the simulation
  Eigen::Vector3f positionVec=earthObj.position.cast<float>();
  earthNode->setPosition(Ogre::Vector3( static_cast<Ogre::Real*>(positionVec.data()) ));
  Eigen::Vector4f attitudeVec=earthObj.attitude.coeffs().cast<float>();
  earthNode->setOrientation(Ogre::Quaternion(static_cast<Ogre::Real*>(attitudeVec.data()) ));

}

bool PropagatorTestApp::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

  // Call the base class event handler
  bool ret = BaseApplication::frameRenderingQueued(evt);

  // Get the time
  double time=(double)timer->getMicroseconds()/1000000.0;
  double dt=time-lastFrameTime;

  // Time acceleration rate
  double speedup=1;

  // Step the simulation
  simMgr.run(lastFrameTime*speedup,time*speedup,dt*speedup);

  // Update the scene
  updateCameraState();
  updateEarthState();

  // Store the time
  lastFrameTime=time;

  return ret;
}

bool PropagatorTestApp::keyPressed( const OIS::KeyEvent &arg )
{  
  return BaseApplication::keyPressed(arg);
}

bool PropagatorTestApp::keyReleased( const OIS::KeyEvent &arg )
{ 
  return BaseApplication::keyReleased(arg);
}

bool PropagatorTestApp::axisMoved( const OIS::JoyStickEvent &arg, int axis )
{
  // Set torquer states according to the joystick position
  for(int i=0; i<3; i++)
    torquers[i]->torque=(double)arg.state.mAxes[i].abs/mJoyStick->MAX_AXIS*0.1;

  return true;
}

// Joystick button handlers (do nothing for now)
bool PropagatorTestApp::buttonPressed( const OIS::JoyStickEvent &arg, int button ){return true;}
bool PropagatorTestApp::buttonReleased( const OIS::JoyStickEvent &arg, int button ){return true;}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
  {
    // Create application object
    PropagatorTestApp app=PropagatorTestApp();

    try {
      app.go();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "An exception has occured: " <<
	e.getFullDescription().c_str() << std::endl;
#endif
    }

    return 0;
  }

#ifdef __cplusplus
}
#endif
