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

void Device::setMessage(Device::message_type mes)
{
  message = mes;
  mit = message.begin();
}

void Device::showNextBit(cv::Mat & image)
{
  if (isNextBit())
  {
    cv::rectangle(image, rect, cv::Scalar(*(mit++) ? 255 : 0), CV_FILLED);
  }
}

bool Device::isNextBit()
{
  return mit != message.end();

}