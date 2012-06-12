#pragma once
/**
  * Screen
  */

#include <opencv2\opencv.hpp>
#include <vector>

class ScreenShape
{
public:
  ScreenShape(void);
  ~ScreenShape(void);

  /** Function that detects white screen on image from camera.
      @param image Image form camera
      */
  bool detectScreen(cv::Mat & image);
  
  void printScreen(cv::Mat & image);

  std::vector<double> getSize() const;

  void findScreenAtBinaryImage(cv::Mat & bin);

  std::vector<cv::Point> getCorners() const;

  std::vector<cv::Point> getScreen() const;

private:
  double thres;
  std::vector<cv::Point> screen;

  std::vector<cv::Point> corners;
};

