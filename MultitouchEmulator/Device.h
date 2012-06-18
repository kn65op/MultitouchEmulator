#pragma once

#include <vector>

//Class representing device.
class Device
{
public:
  Device(void);
  ~Device(void);

  void addPoint(int x, int y);
  
  bool isRealDevice();

private:
  std::vector<int> xs;
  std::vector<int> ys;

};

