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

void Devices::processToTransmition(Key key)
{
  //tmp fake
  /*
  int s = devices.size();
  std::vector<bool> tmp;
  for (int i =0; i < 10 + s; ++i)
  {
    tmp.push_back( i % 3 == 0);
  }
  //tmp fake*/
  
  devices.begin()->second->calcRect();
  devices.begin()->second->setMessage(key.getMasterDeviceCode());

  iterator it, end;
  end = devices.end();
  int i = 0;
  for (it = ++(devices.begin()); it != end; ++it)
  {
    it->second->calcRect();
    it->second->setMessage(key.getSecondaryDeviceCode(++i));
    //it->second->setMessage(std::vector<bool>(tmp.begin() + i, tmp.begin() + 10 + i)); //fake
    //i++; //fake
  }
}