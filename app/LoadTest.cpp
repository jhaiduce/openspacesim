/*
 * LoadTest.cpp
 *
 *  Created on: Mar 21, 2015
 *      Author: jhaiduce
 */
#include "GameManager.hpp"

int main(int argc, char *argv[])
{
  GameManager gm;
  gm.LoadState("/home/jhaiduce/Development/space_simulator/simulator/share/statefile.json");
  //gm.LoadState(argv[1]);
  return 0;
}
