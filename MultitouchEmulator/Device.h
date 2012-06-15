#pragma once

//Class representing device.
class Device
{
public:
  Device(void);
  ~Device(void);

  void addPoint(int x, int y);
  
  bool isRealDevice();


};

