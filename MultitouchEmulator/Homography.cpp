#include "StdAfx.h"
#include "Homography.h"

#include <cmath>
#include <sstream>

Homography::Homography(void)
{
  points_setted = false;
  min_x = min_y = 4000;
  max_x = max_y = 0;
}


Homography::~Homography(void)
{
}

void Homography::runHomography(cv::Mat image_points, cv::Mat real_points)
{
  H = cv::findHomography(image_points, real_points);
}

cv::Point Homography::getRealPoint(int x, int y)
{
  cv::Mat image_point = (cv::Mat_<double>(3,1) << x, y, 1);
  cv::Mat real_point = H * image_point;
  real_point /= real_point.at<double>(2);
  return cv::Point((int)real_point.at<double>(0), (int)real_point.at<double>(1));
}

void getPointsFromImage(int event, int x, int y, int flags, void* param)
{
  static int counter = 0;
  if (counter == 4)
  {
    Homography* h = static_cast<Homography*>(param);
    h->points_setted = true;
    return;
  }
  if (event == CV_EVENT_LBUTTONDOWN)
  {
    Homography* h = static_cast<Homography*>(param);
    std::cout << x << " " << y << "\n";
    if (!counter)
    {
      h->image_points = cv::Mat(4,1, CV_32FC2);
    }
    h->image_points.at<cv::Point2f>(counter++, 0) = cv::Point2f(float(y), float(x));
    h->setMinMax(y, x);
  }
}

void Homography::setGeneratedImageSize(int x, int y)
{
  generated_x = x;
  generated_y = y;
  //generated = cv::Mat(y, x, CV_8UC1);
}

void Homography::setGeneratedImageSize(cv::Size size)
{
  setGeneratedImageSize(size.height, size.width);
  //generated = cv::Mat(y, x, CV_8UC1);
  
  GUI = cv::Mat(generated_x, generated_y, CV_8UC1, cv::Scalar(255));
  int level = (int)(generated_x * 0.1);
  cv::line(GUI, cv::Point(0, level), cv::Point(generated_y, level), cv::Scalar(0), 1);
}

void Homography::runHomography()
{
  real_points = cv::Mat(4,1, CV_32FC2);
  real_points.at<cv::Point2f>(0,0) = cv::Point2f(0,0);
  real_points.at<cv::Point2f>(1,0) = cv::Point2f(0, (float)generated_y);
  real_points.at<cv::Point2f>(2,0) = cv::Point2f((float)generated_x,0);
  real_points.at<cv::Point2f>(3,0) = cv::Point2f((float)generated_x,(float)generated_y);

  runHomography(image_points, real_points);

  makeLUT();
}

bool Homography::isPointsSet()
{
  return points_setted;
}

cv::Mat & Homography::processImage(cv::Mat & image)
{
  generated = cv::Mat(generated_x, generated_y, CV_8UC1, cv::Scalar(255));
  for (int i=min_x - 1 ; i<max_x; ++i)
  {
    for (int j=min_y - 1; j<max_y; ++j)
    {
      cv::Point tmp = LUT.at<cv::Point2f>(i,j);
      if (tmp.x > 0 && tmp.x < generated_x && tmp.y > 0 && tmp.y < generated_y)
      {
        generated.at<uchar>(tmp.x, tmp.y) = image.at<uchar>(i,j);
      }
    }
  }
  return generated;
}

void Homography::setMinMax(int x, int y)
{
  if ( x > max_x)
  {
    max_x = x;
  }
  if ( y > max_y)
  {
    max_y = y;
  }
  if (x < min_x)
  {
    min_x = x;
  }
  if (y < min_y)
  {
    min_y = y;
  }
}

void Homography::setImageSize(int x, int y)
{
  image_x = x;
  image_y = y;
}

void Homography::makeLUT()
{
  LUT = cv::Mat(image_y, image_x, CV_32FC2);
  if (min_x > max_x)
  {
    min_x = 1;
    max_x = image_y;
    min_y = 1;
    max_y = image_x;
  }
  else
  {
    --min_x;
    --min_y;
  }
  for (int i=min_x ; i<max_x; ++i)
  {
    for (int j=min_y; j<max_y; ++j)
    {
      cv::Point tmp = getRealPoint(i,j);
      LUT.at<cv::Point2f>(i, j) = cv::Point2f((float)tmp.x, (float)tmp.y);
    }
  }
}

void Homography::setImageSize(cv::Size size)
{
  setImageSize(size.width, size.height);
}

void Homography::runHomography(cv::Mat image_points)
{
  image_points.copyTo(this->image_points);

  runHomography();
}

void Homography::setROI(cv::Mat & frame) const
{
  frame = cv::Mat(frame, cv::Rect(min_y, min_x, max_y - min_y, max_x - min_x));
}

cv::Mat & Homography::getGUIDetectDevice(Devices & devs)
{
  clearGUI();
  std::stringstream text;
  text << devs.size();
  text << " devices found. Press any key to start transmission";
  cv::putText(GUI, text.str(), cv::Point((int)(generated_y * 0.05), (int)(generated_x * 0.05)), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0));
  return GUI;
}

cv::Mat & Homography::getGUIDetectScreen()
{
  clearGUI();
  cv::putText(GUI, "Press any key when circles points to screen corners", cv::Point((int)(generated_y * 0.05), (int)(generated_x * 0.05)), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0));
  return GUI;
}

void Homography::clearGUI()
{
  int level = (int)(generated_x * 0.1) - 1;
  cv::rectangle(GUI, cv::Rect(0,0,generated_x, level), cv::Scalar(255), CV_FILLED);
}