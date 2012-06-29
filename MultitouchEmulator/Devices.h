#pragma once

#include <map>

#include "Device.h"
#include "Key.h"
#include "Homography.h"

class Devices
{
  //types
private:
  typedef std::map<int, Device*> container;
  typedef std::pair<int, Device*> pair_type;
public:
  typedef std::map<int, Device*>::iterator iterator;

  //members
public:
  Devices(void);
  ~Devices(void);

  /**
    * Function to add pixel from some device.
    * @param i Device ID.
    * @param x x coordinate.
    * @param y y coordinate.
    */
  void setPixel(int i, int x, int y);

  /**
    * Function processing founded devices after all added.
    */
  void processNewScene();

  /**
    * Function clears the list of devices
    */
  void clear();

  /**
    * Function that returns number of devices
    */
  int size() const;

  void processToTransmition(Key key, Homography & hom);

  iterator getBegin();
  iterator getEnd();

  /**
    * Fuction calculates length of the longest message.
    */
  int getLongestMessage();

    /**
    * Fuction returns length of the longest message.
    */
  int getMaxKeyLength() const;
private:
  container devices;

  int key_max_length;
};

