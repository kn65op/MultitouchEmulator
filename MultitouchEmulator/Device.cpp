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
  Device::message_iterator it, end;
  it = mes.begin();
  end = mes.end();

  //Manchester Code
  for (; it != end; ++it)
  {
    if (*it)
    {
      message.push_back(true);
      message.push_back(false);
    }
    else
    {
      message.push_back(false);
      message.push_back(true);
    }
  }

  mit = message.begin();
}

void Device::showNextBit(cv::Mat & image)
{
  if (isNextBit())
  {
    //cv::rectangle(image, rect, cv::Scalar(*(mit++) ? 255 : 0), CV_FILLED);
    cv::rectangle(image, rect, *(mit++) ? cv::Scalar(255,255,255): cv::Scalar(0,0,0), CV_FILLED);
  }
}

bool Device::isNextBit()
{
  return mit != message.end();

}

bool Device::isRealDevice()
{
  return points.size() > 25;
}

int Device::getMessageLength() const
{
  return message.size();
}