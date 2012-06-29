#pragma once

#include <vector>
#include <random>

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
  void showNoiseAround(cv::Mat & image);
  void showRandomBlinkAround(cv::Mat & image);

  int getMessageLength() const;

  void shift(double camera_pos_x, double x, double camera_pos_y, double y);

private:
  std::vector<int> xs;
  std::vector<int> ys;
  Device(const Device & copy);

  std::vector<cv::Point> points;

  cv::Rect rect;
  cv::Rect bigger_rect;

  message_type message;
  message_iterator mit;

  //random
  std::random_device rd;
  std::mt19937 *gen;
  std::uniform_int_distribution<> *dist;
};

