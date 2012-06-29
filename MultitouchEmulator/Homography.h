#pragma once

#include <opencv2\opencv.hpp>

class Devices;

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
  /**
    * Function returns image with GUI for transmission.
    */
  cv::Mat & getGUITransmission(Devices & devs);

  double getShiftX() const;
  double getShiftY() const;

  double getCameraX() const;
  double getCameraY() const;

private:
  cv::Mat H;

  cv::Mat LUT;
  
  cv::Mat GUI;

  cv::Mat image_points;
  cv::Mat real_points;

  int generated_x;
  int generated_y;
  int level;

  bool points_setted;

  cv::Mat generated;

  int image_x, image_y;

  int min_x, max_x, min_y, max_y;
  void setMinMax(int x, int y);

  void clearGUI();

  cv::Scalar colorGUI;
  cv::Scalar color_detect_screen;
  cv::Scalar color_line;

  void setGUIColor(cv::Scalar & scalar);

  void randomBlink();

  //height
  double ratio_x, ratio_y;

  //scale
  double scale, h, h1;
  //camera position
  double camera_pos_x, camera_pos_y;
};
