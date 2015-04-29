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

class GameManager
{
public:
  GameManager();
  SimulationManager environmentManager;
  void LoadState(std::string);
protected:
  std::map<std::basic_string<char>,SpaceObject* (*)(const rapidjson::Value&)> game_object_makers;
};


#endif /* LIBSPACESIM_GAMEMANAGER_HPP_ */
