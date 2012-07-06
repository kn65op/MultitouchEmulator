#pragma once

#include <map>

#include "Device.h"
#include "Key.h"
#include "Homography.h"

/**
 * Class that is container for devices.
 */
class Devices
{
  //types
private:
  // actual container
  typedef std::map<int, Device*> container;
  // pair for container
  typedef std::pair<int, Device*> pair_type;
public:
  /**
   * iterator for devices
   */
  typedef std::map<int, Device*>::iterator iterator;

  //members
public:
  //Default constructor 
  Devices(void);
  //Default destrucor
  ~Devices(void);

  /**
   * Function to add pixel from some device.
   * @param i Device ID.
   * @param x x coordinate.
   * @param y y coordinate.
   */
  void setPixel(int i, int x, int y);

  /**
   * Function processing founded devices (i.e. erases small objects, beacause there are probably noise) after all added.
   */
  void processNewScene();

  /**
   * Function clears the list of devices
   */
  void clear();

  /**
   * 
   * Function that returns number of devices
   * @return Number of devices as int.
   */
  int size() const;

  /**
   * Function adjust and prepare devices to be shown at screen.
   * @param key Key which take care about keys sended through optical channel.
   * @param hom Homography used to recognize devices.
   */
  void processToTransmition(Key key, Homography & hom);

  /**
   * Function returs iteraor to first device.
   * @return Devices::iterator to first device.
   */
  iterator getBegin();
  /**
   * Function returs iteraor to one after last device.
   * @return Devices::iterator to one after last device.
   */
  iterator getEnd();

  /**
   * Fuction calculates length of the longest message.
   * @return Length of the longest message as int.
   */
  int getLongestMessage();

  /**
   * Fuction returns length of the longest message.
   * @return Length of the longest message as int.
   */
  int getMaxKeyLength() const;
private:
  // Conatainer for devices
  container devices;

  // Length of the longest message.
  int key_max_length;
};

