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
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Spacecraft.hpp"
#include "MassiveObject.hpp"
#include "Actuator.hpp"

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
      if(objects[i]["type"]=="Spacecraft")
	{
	  Spacecraft* object=new Spacecraft();
	  object->mass=objects[i]["mass"].GetDouble();
	  for(int j=0; j<3; j++)
	    object->position(j)=objects[i]["position"][j].GetDouble();
	  for(int j=0; j<3; j++)
	    object->velocity(j)=objects[i]["velocity"][j].GetDouble();
	  object->attitude=Eigen::Quaterniond(objects[i]["attitude"][0].GetDouble(),
					     objects[i]["attitude"][1].GetDouble(),
					     objects[i]["attitude"][2].GetDouble(),
					     objects[i]["attitude"][3].GetDouble());
	  for(int j=0; j<3; j++)
	    object->angularVelocity(j)=objects[i]["angularVelocity"][j].GetDouble();

	  for(int j=0; j<objects[i]["actuators"].Size(); j++)
	    {
	      if(objects[i]["actuators"][j]["type"]=="Torquer")
		{
		  Torquer* torquer=new Torquer();
		  for(int k=0; k<3; k++)
		    torquer->Axis(k)=objects[i]["actuators"][j]["axis"][k].GetDouble();
		  object->actuators.push_back(torquer);
		}
	    }

	  environmentManager.addObject(object);
	}
      if(objects[i]["type"]=="MassiveObject")
	{
	  MassiveObject* object=new MassiveObject();
	  object->mass=objects[i]["mass"].GetDouble();
	  for(int j=0; j<3; j++)
	    object->position(j)=objects[i]["position"][j].GetDouble();
	  for(int j=0; j<3; j++)
	    object->velocity(j)=objects[i]["velocity"][j].GetDouble();
	  object->attitude=Eigen::Quaterniond(objects[i]["attitude"][0].GetDouble(),
					     objects[i]["attitude"][1].GetDouble(),
					     objects[i]["attitude"][2].GetDouble(),
					     objects[i]["attitude"][3].GetDouble());
	  for(int j=0; j<3; j++)
	    object->angularVelocity(j)=objects[i]["angularVelocity"][j].GetDouble();

	  environmentManager.addObject(object);
	}
    }
}
