#include "PropagatorInteractionTest.hpp"
#include <OgreSubEntity.h>

//-------------------------------------------------------------------------------------
PropagatorTestApp::PropagatorTestApp(void)
{

  gameMgr=new GameManager(mSceneMgr);
  earthNode=NULL;
  earthObj=NULL;
  scObj=NULL;
  timer=new Ogre::Timer();
  lastFrameTime=0;

  // Time acceleration rate
  speedup=1;
}
//-------------------------------------------------------------------------------------
PropagatorTestApp::~PropagatorTestApp(void)
{
  delete gameMgr;
  delete timer;
}
//-------------------------------------------------------------------------------------
bool PropagatorTestApp::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();

    // Setup material manager
    Ogre::MaterialManager & materialManager = Ogre::MaterialManager::getSingleton();
    Ogre::String resourceGroupName = "objTextures";
    Ogre::ResourceGroupManager & rgMgr = Ogre::ResourceGroupManager::getSingleton();
    rgMgr.createResourceGroup(resourceGroupName);
    Ogre::String texDirPath = "materials";
    bool isRecursive = false;
    rgMgr.addResourceLocation(texDirPath, "FileSystem", resourceGroupName, isRecursive);
    rgMgr.initialiseResourceGroup(resourceGroupName);
    rgMgr.loadResourceGroup(resourceGroupName);
    Ogre::MaterialPtr earthMat = materialManager.create("EarthSimple",resourceGroupName);
    Ogre::MaterialPtr stationMat = materialManager.create("Material",resourceGroupName);

    gameMgr->SetSceneManager(mSceneMgr);

    gameMgr->LoadState("../share/statefile.json");

    mCamera=gameMgr->getCamera(0)->camera;

    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();

    createFrameListener();

    return true;
};

//-------------------------------------------------------------------------------------
void PropagatorTestApp::createScene(void)
{

  // Set the scene's ambient light
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

  scObj=(Spacecraft*)gameMgr->environmentManager.getObject(0);
  earthObj=(MassiveObject*)gameMgr->environmentManager.getObject(1);

  earthNode=gameMgr->GetEntity(0)->GetNode();
  stationNode=gameMgr->GetEntity(1)->GetNode();

  // Create SkyBox
  mSceneMgr->setSkyBox(true,"SkyBox");

  // Create a Light and set its position
  Ogre::Light* light = mSceneMgr->createLight("MainLight");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(0,0,1));
  //light->setPosition(20000000.0f, 80000000.0f, 50000000.0f);

  // Initialize the timer
  timer->reset();
  lastFrameTime=0;
}

bool PropagatorTestApp::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

  // Call the base class event handler
  bool ret = BaseApplication::frameRenderingQueued(evt);

  // Get the time
  double time=(double)timer->getMicroseconds()/1000000.0;
  double dt=time-lastFrameTime;

  // Step the simulation
  gameMgr->run(lastFrameTime*speedup,time*speedup);
  gameMgr->updateNodePositions();
  gameMgr->updateCameraPositions();

  // Store the time
  lastFrameTime=time;

  return ret;
}

bool PropagatorTestApp::keyPressed( const OIS::KeyEvent &arg )
{  
    
  if (arg.key==OIS::KC_LEFT) ((Torquer*)scObj->actuators[0])->torque=-0.5;
    
  if (arg.key==OIS::KC_RIGHT) ((Torquer*)scObj->actuators[0])->torque=0.5;
    
  if (arg.key==OIS::KC_UP) ((Torquer*)scObj->actuators[1])->torque=-0.5;
    
  if (arg.key==OIS::KC_DOWN) ((Torquer*)scObj->actuators[1])->torque=0.5;

  if (arg.key==OIS::KC_PGUP) ((Torquer*)scObj->actuators[2])->torque=0.5;
    
  if (arg.key==OIS::KC_PGDOWN) ((Torquer*)scObj->actuators[2])->torque=-0.5;

  if(arg.key==OIS::KC_SPACE) ((Thruster*)scObj->actuators[3])->force=1;

  return BaseApplication::keyPressed(arg);
}

bool PropagatorTestApp::keyReleased( const OIS::KeyEvent &arg )
{ 
  if (arg.key==OIS::KC_LEFT) ((Torquer*)scObj->actuators[0])->torque=0;
    
  if (arg.key==OIS::KC_RIGHT) ((Torquer*)scObj->actuators[0])->torque=0;
    
  if (arg.key==OIS::KC_UP) ((Torquer*)scObj->actuators[1])->torque=0;
    
  if (arg.key==OIS::KC_DOWN) ((Torquer*)scObj->actuators[1])->torque=0;

  if (arg.key==OIS::KC_PGUP) ((Torquer*)scObj->actuators[2])->torque=0;
    
  if (arg.key==OIS::KC_PGDOWN) ((Torquer*)scObj->actuators[2])->torque=0;

  if(arg.key==OIS::KC_SPACE) ((Thruster*)scObj->actuators[3])->force=0;

  return BaseApplication::keyReleased(arg);
}

bool PropagatorTestApp::axisMoved( const OIS::JoyStickEvent &arg, int axis )
{
  // Set torquer states according to the joystick position
  for(int i=0; i<3; i++)
    ((Torquer*)scObj->actuators[i])->torque=(double)arg.state.mAxes[i].abs/mJoyStick->MAX_AXIS*0.1;

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
