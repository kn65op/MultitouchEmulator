#pragma once

#include <vector>
#include <random>

#include <opencv2\opencv.hpp>

/**
 * Class representing device.
 */
class Device
{
  /**
   * Types
   */
public:
  //type of message
  typedef std::vector<bool> message_type;

private:
  //message iterator
  typedef message_type::iterator message_iterator;

  //members
public:
  /**
   * Default constructor
   */
  Device(void);
  /**
   * Default destructor
   */
  ~Device(void);

  /**
   * Function add point to device.
   * @param x X coordinate of point.
   * @param y Y coordinate of point.
   */
  void addPoint(int x, int y);

  /**
   * Function returns rectangle corresponding to device, after shifting. Before use this fuction you have to calculate this rectangle.
   * @return cv::Rect corresponding to device.
   */
  cv::Rect getRect() const;

  /**
   * Function check if device can  be real device, not noise.
   * @return true if device is big enough to be device, false otherwise.
   */
  bool isRealDevice();

  /**
   * Function calculates rectangle which corrspond to device.
   */
  void calcRect();

  /**
   * Function to set message transmitted via optical channel.
   * @param mes
   */
  void setMessage(message_type mes);
  /**
   * Function returns next bit to transmit.
   * @return Next bit as bool.
   */
  bool getNextBit();
  /**
   * Function to check if some bit to transmit left.
   * @return true if there is more bits to transmit, false otherwise.
   */
  bool isNextBit();
  /**
   * Function show next bit on image in place where device is.
   * @param image Image on which bit will be shown.
   */
  void showNextBit(cv::Mat & image);
  /**
   * Function show wrong bit instead of next bit on image in place where device is.
   * @param image Image on which bit will be shown.
   */
  void showWrongBit(cv::Mat & image);
  /**
   * Function show noise around place where device is. This noise is monochromatic.
   * @param image Image on which noise will be shown.
   */
  void showNoiseAround(cv::Mat & image);
  /**
   * Function show noise around place where device is. This noise is not monochromatic.
   * @param image Image on which noise will be shown.
   */
  void showRandomBlinkAround(cv::Mat & image);

  /**
   * Function returns message length.
   * @return Message length as int.
   */
  int getMessageLength() const;

  /**
   * Function that move device to correct position due to perspective.
   * @param camera_pos_x Camera positon in X coordinate.
   * @param x Parameter to shift device in X coordinate.
   * @param camera_pos_y Camera positon in Y coordinate.
   * @param y Parameter to shift device in Y coordinate.
   */
  void shift(double camera_pos_x, double x, double camera_pos_y, double y);

private:
  // points of device
  std::vector<int> xs;
  std::vector<int> ys;
  Device(const Device & copy);

  std::vector<cv::Point> points;

  //rectangles correspond to device
  cv::Rect rect;
  cv::Rect bigger_rect;

  //message
  message_type message;
  message_iterator mit;

  //random
  std::random_device rd;
  std::mt19937 *gen;
  std::uniform_int_distribution<> *dist;
};

