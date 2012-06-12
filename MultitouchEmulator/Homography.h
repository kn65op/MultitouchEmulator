#pragma once

#include <opencv2\opencv.hpp>

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

  bool isPointsSet();

  cv::Mat & processImage(cv::Mat & image);

  void setImageSize(int x, int y);
  void setImageSize(cv::Size size);

  void makeLUT();
private:
  cv::Mat H;

  cv::Mat LUT;

  cv::Mat image_points;
  cv::Mat real_points;

  int generated_x;
  int generated_y;

  bool points_setted;

  cv::Mat generated;

  int image_x, image_y;

  int min_x, max_x, min_y, max_y;
  void setMinMax(int x, int y);
};
