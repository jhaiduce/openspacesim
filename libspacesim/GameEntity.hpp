/*
 * GameEntity.hpp
 *
 *  Created on: May 2, 2015
 *      Author: jhaiduce
 */

#ifndef LIBSPACESIM_GAMEENTITY_HPP_
#define LIBSPACESIM_GAMEENTITY_HPP_

#include <OgreNode.h>
#include <OgreEntity.h>
#include "SimulationObject.hpp"
#include "SpaceObject.hpp"

class GameEntity: public SimulationObject
{
public:
  GameEntity(SpaceObject* spaceobj, Ogre::Entity* entity, Ogre::Node* node): spaceobj(spaceobj), entity(entity), node(node){};
  void updateTransformation();
  Ogre::Node* GetNode(){return node;};
  Ogre::Entity* GetEntity(){return entity;};
  SpaceObject* GetSpaceObject(){return spaceobj;};
protected:
  SpaceObject* spaceobj;
  Ogre::Entity* entity;
  Ogre::Node* node;
};



#endif /* LIBSPACESIM_GAMEENTITY_HPP_ */
