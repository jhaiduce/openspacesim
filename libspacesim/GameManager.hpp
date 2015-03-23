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

class GameManager
{
public:
  SimulationManager environmentManager;
  void LoadState(std::string);
};


#endif /* LIBSPACESIM_GAMEMANAGER_HPP_ */
