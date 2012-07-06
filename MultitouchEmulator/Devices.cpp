#include "StdAfx.h"
#include "Devices.h"

#include <algorithm>


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
  iterator it, end;
  end = devices.end();
  for (it = devices.begin(); it!=end;)
  {
    //check if devices can be found as real device
    if (it->second->isRealDevice())
    {
      ++it;
    }
    else
    {
      it = devices.erase(it);
    }
  }
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

void Devices::shiftDevices(Homography & hom)
{
  iterator it, end;
  end = devices.end();
  for (it = devices.begin(); it != end; ++it)
  {
    it->second->shift(hom.getCameraX(), hom.getShiftX(), hom.getCameraY(),  hom.getShiftY());
  }
}

void Devices::processToTransmition(Key key, Homography & hom)
{  
  //calculate rectangle and set message for coordinator
  devices.begin()->second->calcRect();
  devices.begin()->second->setMessage(key.getMasterDeviceCode());

  //calculate rectangle and set message for other devices
  iterator it, end;
  end = devices.end();
  int i = 0;
  for (it = ++(devices.begin()); it != end; ++it)
  {
    it->second->calcRect();
    it->second->setMessage(key.getSecondaryDeviceCode(++i));
  }

  key_max_length = getLongestMessage();
}

int Devices::getMaxKeyLength() const
{
  return key_max_length;
}

int Devices::getLongestMessage()
{
  int max = 0;
  int tmp;

  iterator it, end;
  end = devices.end();
  for (it = devices.begin(); it != end; ++it)
  {
    tmp = it->second->getMessageLength();
    if (tmp > max)
    {
      max = tmp;
    }
  }

  return max;
}