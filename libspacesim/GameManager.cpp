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

SpaceObject* makeMassiveObject(const rapidjson::Value& jsonobj){
  MassiveObject* object=new MassiveObject();
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
  return (SpaceObject*)object;
}

SpaceObject* makeSpacecraft(const rapidjson::Value& jsonobj){
  Spacecraft* object=new Spacecraft();
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
      if(jsonobj["actuators"][j]["type"]=="Torquer")
	  {
	    Torquer* torquer=new Torquer();
	    for(int k=0; k<3; k++)
	      torquer->Axis(k)=jsonobj["actuators"][j]["axis"][k].GetDouble();
	    object->actuators.push_back(torquer);
	  }
    }
  return (SpaceObject*)object;
}

GameManager::GameManager(){
  game_object_makers["Spacecraft"]=&makeSpacecraft;
  game_object_makers["MassiveObject"]=&makeMassiveObject;
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
      SpaceObject* (*object_maker)(const rapidjson::Value&)=game_object_makers.at(typestr);
      SpaceObject*object=object_maker(objects[i]);
      environmentManager.addObject(object);
    }
}
