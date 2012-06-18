#pragma once

#include <opencv2\opencv.hpp>

#include "Devices.h"

class Homography
{
public:
  Homography(void);
  ~Homography(void);
  
  void runHomography(cv::Mat image_points, cv::Mat real_points);
  void runHomography();
  void runHomography(cv::Mat image_points);

  cv::Point getRealPoint(int x, int y);

  friend void getPointsFromImage(int event, int x, int y, int flags, void* param);

  void setGeneratedImageSize(int x, int y);
  void setGeneratedImageSize(cv::Size size);

  bool isPointsSet();

  cv::Mat & processImage(cv::Mat & image);

  void setImageSize(int x, int y);
  void setImageSize(cv::Size size);

  void setROI(cv::Mat & frame) const;

  void makeLUT();

  /**
    * Function returns image with GUI for detecting devices.
    */
  cv::Mat & getGUIDetectDevice(Devices & devs);
  /**
    * Function returns image with GUI for detecting screen.
    */
  cv::Mat & getGUIDetectScreen();

private:
  cv::Mat H;

  cv::Mat LUT;
  
  cv::Mat GUI;

  cv::Mat image_points;
  cv::Mat real_points;

  int generated_x;
  int generated_y;

  bool points_setted;

  cv::Mat generated;

  int image_x, image_y;

  int min_x, max_x, min_y, max_y;
  void setMinMax(int x, int y);

  void clearGUI();
};
