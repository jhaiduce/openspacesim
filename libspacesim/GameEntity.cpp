/*
 * GameEntity.cpp
 *
 *  Created on: May 2, 2015
 *      Author: jhaiduce
 */




#include "GameEntity.hpp"

void GameEntity::updateTransformation()
{
  Eigen::Vector3f positionVec=spaceobj->position.cast<float>();
  node->setPosition(Ogre::Vector3( static_cast<Ogre::Real*>(positionVec.data()) ));
  Eigen::Vector4f attitudeVec=spaceobj->attitude.coeffs().cast<float>();
  node->setOrientation(Ogre::Quaternion(static_cast<Ogre::Real*>(attitudeVec.data()) ));
}
