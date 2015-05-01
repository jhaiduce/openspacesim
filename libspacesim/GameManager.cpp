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
  object->attitude=Eigen::Quaterniond(jsonobj["attitude"][0].GetDouble(),
				      jsonobj["attitude"][1].GetDouble(),
				      jsonobj["attitude"][2].GetDouble(),
				      jsonobj["attitude"][3].GetDouble());
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
  object->attitude=Eigen::Quaterniond(jsonobj["attitude"][0].GetDouble(),
				       jsonobj["attitude"][1].GetDouble(),
				       jsonobj["attitude"][2].GetDouble(),
				       jsonobj["attitude"][3].GetDouble());
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

GameManager::GameManager(){

  game_object_makers["Spacecraft"]=new SpacecraftFactory(this);
  game_object_makers["MassiveObject"]=new MassiveObjectFactory(this);
  game_object_makers["Torquer"]=new TorquerFactory(this);
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
}
