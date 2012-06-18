#pragma once

#include <map>

#include "Device.h"

class Devices
{
private:
  typedef std::map<int, Device*> container;
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
private:
  container devices;
};

