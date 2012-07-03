#include "StdAfx.h"
#include "Device.h"


Device::Device(void)
{
  //start random engine
  gen = new std::mt19937(rd());
  dist = new std::uniform_int_distribution<>(0, 255);
}


Device::~Device(void)
{
  delete gen;
  delete dist;
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

void Device::shift(double camera_pos_x, double x, double camera_pos_y, double y)
{
  std::vector<cv::Point>::iterator it, end;
  end = points.end();
  for (it = points.begin(); it != end; ++it)
  {
    it->x = static_cast<int>(camera_pos_x + (it->x - camera_pos_x) * x);
    it->y = static_cast<int>(camera_pos_y + (it->y - camera_pos_y) * y);
  }
}

void Device::calcRect()
{
  //finding rectangle (not rotated)
  rect = cv::boundingRect(points);
  bigger_rect.x  = rect.x - 30;
  bigger_rect.y  = rect.y - 30;
  bigger_rect.width  = rect.width + 60;
  bigger_rect.height = rect.height + 60;
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

void Device::showNoiseAround(cv::Mat & image)
{
  cv::rectangle(image, bigger_rect, cv::Scalar((*dist)(*gen),(*dist)(*gen),(*dist)(*gen)), CV_FILLED);
}

void Device::showRandomBlinkAround(cv::Mat & image)
{
  cv::rectangle(image, bigger_rect, (*dist)(*gen) > 127 ? cv::Scalar(255,255,255): cv::Scalar(0,0,0), CV_FILLED);
}

void Device::showNextBit(cv::Mat & image)
{
  if (isNextBit())
  {
    cv::rectangle(image, rect, *(mit++) ? cv::Scalar(255,255,255): cv::Scalar(0,0,0), CV_FILLED);
  }
}

void Device::showWrongBit(cv::Mat & image)
{
  if (isNextBit())
  {
    cv::rectangle(image, rect, *(mit++) ? cv::Scalar(0,0,0): cv::Scalar(255,255,255), CV_FILLED);
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