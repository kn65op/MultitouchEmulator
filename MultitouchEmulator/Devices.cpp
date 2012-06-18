#include "StdAfx.h"
#include "Devices.h"


Devices::Devices(void)
{
}


Devices::~Devices(void)
{
}

void Devices::setPixel(int i, int x, int y)
{
  //device doesn't exist
  if (devices.find(i) == devices.end())
  {
    devices[i] = new Device();
  }
  devices[i]->addPoint(x,y);
}

void Devices::processNewScene()
{

}

void Devices::clear()
{
  iterator it, end;
  end = devices.end();
  for (it = devices.begin(); it != end; ++it)
  {
    delete it->second;
  }
  devices.clear();
}

int Devices::size() const
{
  return devices.size();
}

Devices::iterator Devices::getBegin()
{
  return devices.begin();
}

Devices::iterator Devices::getEnd()
{
  return devices.end();
}

void Devices::processToTransmition()
{
  iterator it, end;
  end = devices.end();
  for (it = devices.begin(); it != end; ++it)
  {
    it->second->calcRect();
  }
}