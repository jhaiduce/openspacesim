/*
 * GameManager.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: jhaiduce
 */

#include "GameManager.hpp"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Spacecraft.hpp"
#include "MassiveObject.hpp"
#include "Actuator.hpp"

SimulationObject* MassiveObjectFactory::make(const rapidjson::Value& jsonobj){
  MassiveObject* object=new MassiveObject();
  this->game_manager->add_allocated_object(object);
  object->mass=jsonobj["mass"].GetDouble();
  for(int j=0; j<3; j++)
    object->position(j)=jsonobj["position"][j].GetDouble();
  for(int j=0; j<3; j++)
    object->velocity(j)=jsonobj["velocity"][j].GetDouble();
  object->attitude=Eigen::Quaterniond(jsonobj["attitude"][0u].GetDouble(),
				      jsonobj["attitude"][1u].GetDouble(),
				      jsonobj["attitude"][2u].GetDouble(),
				      jsonobj["attitude"][3u].GetDouble());
  for(int j=0; j<3; j++)
    object->angularVelocity(j)=jsonobj["angularVelocity"][j].GetDouble();
  return (SimulationObject*)object;
}

SimulationObject* SpacecraftFactory::make(const rapidjson::Value& jsonobj){
  Spacecraft* object=new Spacecraft();
  this->game_manager->add_allocated_object(object);
  object->mass=jsonobj["mass"].GetDouble();
  for(int j=0; j<3; j++)
    object->position(j)=jsonobj["position"][j].GetDouble();
  for(int j=0; j<3; j++)
    object->velocity(j)=jsonobj["velocity"][j].GetDouble();
  object->attitude=Eigen::Quaterniond(jsonobj["attitude"][0u].GetDouble(),
				       jsonobj["attitude"][1u].GetDouble(),
				       jsonobj["attitude"][2u].GetDouble(),
				       jsonobj["attitude"][3u].GetDouble());
  for(int j=0; j<3; j++)
    object->angularVelocity(j)=jsonobj["angularVelocity"][j].GetDouble();

  for(int j=0; j<jsonobj["actuators"].Size(); j++)
    {
      SimulationObjectFactory*factory=this->game_manager->get_factory(jsonobj["actuators"][j]["type"].GetString());
      SimulationObject* actuator=factory->make(jsonobj["actuators"][j]);
      object->actuators.push_back((Actuator*)actuator);
    }
  return (SpaceObject*)object;
}

SimulationObject* TorquerFactory::make(const rapidjson::Value& jsonobj){
  Torquer* torquer=new Torquer();
  this->game_manager->add_allocated_object(torquer);
  for(int k=0; k<3; k++)
    torquer->Axis(k)=jsonobj["axis"][k].GetDouble();
  return (SimulationObject*)torquer;
}

SimulationObject* ThrusterFactory::make(const rapidjson::Value& jsonobj){
  Thruster* thruster=new Thruster();
  this->game_manager->add_allocated_object(thruster);
  for(int k=0; k<3; k++)
    thruster->Axis(k)=jsonobj["axis"][k].GetDouble();
  return (SimulationObject*)thruster;
}

GameEntity* SphereFactory::make(const rapidjson::Value& jsonobj, SpaceObject*spaceobj){

  // Create the sphere
  Ogre::Entity* entity = game_manager->GetSceneManager()->createEntity(jsonobj["name"].GetString(), Ogre::SceneManager::PT_SPHERE);
  entity->setMaterialName(jsonobj["material"].GetString());

  // Create a SceneNode and attach the Entity to it
  Ogre::SceneNode *node = game_manager->GetSceneManager()->getRootSceneNode()->createChildSceneNode(jsonobj["name"].GetString());
  node->attachObject(entity);

  // Set the node's position
  Eigen::Vector3f positionVec=spaceobj->position.cast<float>();
  node->setPosition(Ogre::Vector3( static_cast<Ogre::Real*>(positionVec.data()) ));

  // Set the node's orientation
  Eigen::Vector4f attitudeVec=spaceobj->attitude.coeffs().cast<float>();
  node->setOrientation(Ogre::Quaternion(static_cast<Ogre::Real*>(attitudeVec.data()) ));

  // Scale the sphere
  node->scale( jsonobj["scale"][0u].GetDouble(), jsonobj["scale"][1u].GetDouble(),jsonobj["scale"][2u].GetDouble() );

  GameEntity*game_entity=new GameEntity(spaceobj,entity,node);
  this->game_manager->add_allocated_object(game_entity);

  return game_entity;
}

GameEntity* MeshFactory::make(const rapidjson::Value& jsonobj, SpaceObject*spaceobj){

  // Create the sphere
  Ogre::Entity* entity = game_manager->GetSceneManager()->createEntity(jsonobj["name"].GetString(), jsonobj["filename"].GetString());
  entity->setMaterialName(jsonobj["material"].GetString());

  // Create a SceneNode and attach the Entity to it
  Ogre::SceneNode *node = game_manager->GetSceneManager()->getRootSceneNode()->createChildSceneNode(jsonobj["name"].GetString());
  node->attachObject(entity);

  // Set the node's position
  Eigen::Vector3f positionVec=spaceobj->position.cast<float>();
  node->setPosition(Ogre::Vector3( static_cast<Ogre::Real*>(positionVec.data()) ));

  // Set the node's orientation
  Eigen::Quaterniond attitude=spaceobj->attitude;
  node->setOrientation(Ogre::Quaternion(attitude.w(),attitude.x(),attitude.y(),attitude.z()));

  // Scale the object
  node->scale( jsonobj["scale"][0u].GetDouble(), jsonobj["scale"][1u].GetDouble(),jsonobj["scale"][2u].GetDouble() );

  GameEntity*game_entity=new GameEntity(spaceobj,entity,node);
  this->game_manager->add_allocated_object(game_entity);

  return game_entity;
}

GameCamera* CameraFactory::make(const rapidjson::Value& jsonobj){

  // Create the sphere
  Ogre::Camera* camera = game_manager->GetSceneManager()->createCamera(jsonobj["name"].GetString());

  SpaceObject*spaceobj=game_manager->getSpaceObject(jsonobj["attached_object"].GetString());

  // Set the node's position
  Eigen::Vector3f positionVec=spaceobj->position.cast<float>();
  camera->setPosition(Ogre::Vector3( static_cast<Ogre::Real*>(positionVec.data()) ));

  // Set the node's orientation
  Eigen::Quaterniond attitude=spaceobj->attitude;
  camera->setOrientation(Ogre::Quaternion(attitude.w(),attitude.x(),attitude.y(),attitude.z() ));

  camera->setNearClipDistance(jsonobj["near_clip"].GetDouble());
  camera->setFarClipDistance(jsonobj["far_clip"].GetDouble());

  GameCamera* gamecamera=new GameCamera(camera,spaceobj);

  return gamecamera;
}

GameManager::GameManager(Ogre::SceneManager*scene_manager) : scene_manager(scene_manager){
  initializeFactories();
}

GameManager::GameManager() {
  initializeFactories();
  scene_manager=NULL;
}

void GameManager::initializeFactories(){
  game_object_makers["Spacecraft"]=new SpacecraftFactory(this);
  game_object_makers["MassiveObject"]=new MassiveObjectFactory(this);
  game_object_makers["Torquer"]=new TorquerFactory(this);
  game_object_makers["Thruster"]=new ThrusterFactory(this);
  game_entity_makers["Sphere"]=new SphereFactory(this);
  game_entity_makers["Mesh"]=new MeshFactory(this);
  camera_maker=new CameraFactory(this);
}

void GameManager::LoadState(std::string filename)
{
  // Open file
  FILE*fh=fopen(filename.c_str(),"r");

  // Create input stream
  char readBuffer[65536];
  rapidjson::FileReadStream inputStream(fh,readBuffer,sizeof(readBuffer));

  // Parse file
  rapidjson::Document doc;
  doc.ParseStream(inputStream);

  fclose(fh);

  // Print any errors that occurred
  std::cout<<rapidjson::GetParseError_En(doc.GetParseError())<<std::endl;

  // Reset simulation manager
  environmentManager.removeAllObjects();

  // Create simulation objects
  const rapidjson::Value& environment=doc["Environment"];
  const rapidjson::Value& objects=doc["Environment"]["objects"];
  for(rapidjson::SizeType i=0; i<objects.Size(); i++)
    {
      std::string typestr=objects[i]["type"].GetString();
      SimulationObjectFactory*factory=game_object_makers.at(typestr);
      SimulationObject*object=factory->make(objects[i]);
      environmentManager.addObject((SpaceObject*)object);
      space_objects[objects[i]["name"].GetString()]=(SpaceObject*)object;

      if(scene_manager!=NULL)
	{
	  rapidjson::Value::ConstMemberIterator entitr = objects[i].FindMember("entity");
	  if(entitr!=objects[i].MemberEnd())
	    {
	      const rapidjson::Value& entityJson=entitr->value;
	      std::string entity_type=entityJson["type"].GetString();
	      GameEntityFactory*entity_factory=game_entity_makers.at(entity_type);
	      GameEntity*entity=entity_factory->make(entityJson,(SpaceObject*)object);
	      game_entities.push_back(entity);
	    }
	}
    }
  const rapidjson::Value& cameras_json=doc["Environment"]["cameras"];
  if(scene_manager!=NULL)
    {
      for(rapidjson::SizeType i=0; i<cameras_json.Size(); i++)
	{
	  GameCamera*camera=camera_maker->make(cameras_json[i]);
	  cameras.push_back(camera);
	}
      defaultCamera=cameras[0];
    }
}

void GameManager::run(double t0,double t1){
  environmentManager.run(t0,t1,t1-t0);
}

void GameManager::updateCameraPositions()
{
  for(std::vector<GameCamera*>::iterator i=cameras.begin(); i<cameras.end(); i++)
    {
      // Move space station to the location calculated by the simulation
      Eigen::Vector3f positionVec=((*i)->spaceobj->position-defaultCamera->spaceobj->position).cast<float>();
      (*i)->camera->setPosition(Ogre::Vector3( static_cast<Ogre::Real*>(positionVec.data()) ));
      Eigen::Quaterniond attitude=(*i)->spaceobj->attitude;
      (*i)->camera->setOrientation(Ogre::Quaternion(attitude.w(),attitude.x(),attitude.y(),attitude.z()));
    }
}

void GameManager::updateNodePositions()
{
  for(std::vector<GameEntity*>::iterator i=game_entities.begin(); i<game_entities.end(); i++)
    {
      // Move space station to the location calculated by the simulation
      Eigen::Vector3f positionVec=((*i)->GetSpaceObject()->position-defaultCamera->spaceobj->position).cast<float>();
      (*i)->GetNode()->setPosition(Ogre::Vector3( static_cast<Ogre::Real*>(positionVec.data()) ));
      Eigen::Quaterniond attitude=(*i)->GetSpaceObject()->attitude;
      (*i)->GetNode()->setOrientation(Ogre::Quaternion(attitude.w(),attitude.x(),attitude.y(),attitude.z()));
    }
}

GameManager::~GameManager(){
  for(std::vector<SimulationObject*>::iterator i=allocated_objects.begin(); i!=allocated_objects.end(); i++)
    {
      delete *i;
    }
  for(std::map<std::string,SimulationObjectFactory*>::iterator i=game_object_makers.begin(); i!=game_object_makers.end(); i++)
    {
      delete i->second;
    }
  for(std::map<std::string,GameEntityFactory*>::iterator i=game_entity_makers.begin(); i!=game_entity_makers.end(); i++)
    {
      delete i->second;
    }
}
