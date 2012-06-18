#pragma once

#include <vector>

#include <opencv2\opencv.hpp>

//Class representing device.
class Device
{
  //types
public:
  typedef std::vector<bool> message_type;

private:
  typedef message_type::iterator message_iterator;

  //members
public:
  Device(void);
  ~Device(void);

  void addPoint(int x, int y);

  cv::Rect getRect() const;
  
  bool isRealDevice();

  void calcRect();
  
  void setMessage(message_type mes);
  bool getNextBit();
  bool isNextBit();
  void showNextBit(cv::Mat & image);

private:
  std::vector<int> xs;
  std::vector<int> ys;

  std::vector<cv::Point> points;

  cv::Rect rect;

  message_type message;
  message_iterator mit;
};

