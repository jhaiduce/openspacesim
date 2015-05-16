/*
 * GameManager.hpp
 *
 *  Created on: Mar 3, 2015
 *      Author: jhaiduce
 */

#ifndef LIBSPACESIM_GAMEMANAGER_HPP_
#define LIBSPACESIM_GAMEMANAGER_HPP_

#include "SimulationManager.hpp"
#include <string>
#include <map>
#include "rapidjson/document.h"
#include "GameEntity.hpp"
#include <OgreSceneManager.h>

class GameManager;

class SimulationObjectFactory: public SimulationObject
{
public:
  SimulationObjectFactory(GameManager* game_manager) : game_manager(game_manager) {};
  virtual SimulationObject*make(const rapidjson::Value& jsonobj)=0;
  virtual ~SimulationObjectFactory(){};
protected:
  GameManager *game_manager;
};

class MassiveObjectFactory: public SimulationObjectFactory
{
public:
  MassiveObjectFactory(GameManager* game_manager) : SimulationObjectFactory(game_manager) {};
  SimulationObject*make(const rapidjson::Value& jsonobj);
};

class SpacecraftFactory: public SimulationObjectFactory
{
public:
  SpacecraftFactory(GameManager* game_manager) : SimulationObjectFactory(game_manager) {};
  SimulationObject*make(const rapidjson::Value& jsonobj);
};

class TorquerFactory: public SimulationObjectFactory
{
public:
  TorquerFactory(GameManager* game_manager) : SimulationObjectFactory(game_manager) {};
  SimulationObject*make(const rapidjson::Value& jsonobj);
};

class ThrusterFactory: public SimulationObjectFactory
{
public:
  ThrusterFactory(GameManager* game_manager) : SimulationObjectFactory(game_manager) {};
  SimulationObject*make(const rapidjson::Value& jsonobj);
};

class GameEntityFactory
{
public:
  GameEntityFactory(GameManager* game_manager) : game_manager(game_manager) {};
  virtual GameEntity*make(const rapidjson::Value& jsonobj, SpaceObject*spaceobject)=0;
  virtual ~GameEntityFactory(){};
protected:
  GameManager *game_manager;
};

class MeshFactory : public GameEntityFactory
{
public:
  MeshFactory(GameManager* game_manager) : GameEntityFactory(game_manager) {};
  GameEntity*make(const rapidjson::Value& jsonobj, SpaceObject*spaceobject);
};

class SphereFactory : public GameEntityFactory
{
public:
  SphereFactory(GameManager* game_manager) : GameEntityFactory(game_manager) {};
  GameEntity*make(const rapidjson::Value& jsonobj, SpaceObject*spaceobject);
};

class GameCamera{
public:
  GameCamera(Ogre::Camera*camera, SpaceObject*spaceobj): camera(camera), spaceobj(spaceobj) {};
  Ogre::Camera*camera;
  SpaceObject*spaceobj;
};

class CameraFactory
{
public:
  CameraFactory(GameManager* game_manager) : game_manager(game_manager) {};
  GameCamera*make(const rapidjson::Value& jsonobj);
protected:
  GameManager *game_manager;
};

class GameManager
{
public:
  GameManager(Ogre::SceneManager*scene_manager);
  GameManager();
  SimulationManager environmentManager;
  void LoadState(std::string);
  void add_allocated_object(SimulationObject* object)
  {
    allocated_objects.push_back(object);
  }
  SimulationObjectFactory* get_factory(std::string type_name)
  {
    return game_object_makers[type_name];
  }
  ~GameManager();
  Ogre::SceneManager* GetSceneManager(){return scene_manager;};
  GameEntity* GetEntity(int i){return game_entities[i];};
  void SetSceneManager(Ogre::SceneManager*scene_manager){this->scene_manager=scene_manager;};
  void run(double t0,double t1);
  void updateNodePositions();
  void updateCameraPositions();
  GameCamera* getCamera(int i){return cameras[i];};
  SpaceObject* getSpaceObject(std::string name){return space_objects[name];};
protected:
  std::map<std::string,SimulationObjectFactory*> game_object_makers;
  std::map<std::string,GameEntityFactory*> game_entity_makers;
  std::map<std::string,SpaceObject*> space_objects;
  std::vector<SimulationObject*> allocated_objects;
  std::vector<GameEntity*> game_entities;
  std::vector<GameCamera*> cameras;
  Ogre::SceneManager* scene_manager;
  CameraFactory* camera_maker;
  GameCamera* defaultCamera;
private:
  void initializeFactories();
};


#endif /* LIBSPACESIM_GAMEMANAGER_HPP_ */
