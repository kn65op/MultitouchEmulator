#include "StdAfx.h"
#include "Device.h"


Device::Device(void)
{
}


Device::~Device(void)
{
}

void Device::addPoint(int x, int y)
{
  xs.push_back(x);
  ys.push_back(y);
  points.push_back(cv::Point(y, x));
}

cv::Rect Device::getRect() const
{
  return rect;
}

void Device::calcRect()
{
  rect = cv::boundingRect(points);
}