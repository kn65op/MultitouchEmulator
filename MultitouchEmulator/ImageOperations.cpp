#include "stdafx.h"

#include "ImageOperations.h"

void subtractAbs(cv::Mat & s1, cv::Mat & s2, cv::Mat & dest)
{

  cv::Size size = s1.size();
  dest = cv::Mat(size, CV_8UC1);

  for (int i=0; i<size.width; ++i)
  {
    for (int j=0; j<size.height; ++j)
    {
      dest.at<uchar>(j,i) = abs(s1.at<uchar>(j,i) - s2.at<uchar>(j,i)) * 2;
    }
  }
  
}

void createStripesImage(cv::Mat & dest, int height, int width, int stripe_size, int number)
{
  int color = number;
  if (color) color = 255;
  dest.create(width, height, CV_8UC1);
  for (int i=0; i<width; ++i)
  {
    color = number;
    for (int j=0; j<height; ++j)
    {
      if (j % stripe_size == 0)
      {
        color = !color;
        if (color) color = 255;
      }
      dest.at<uchar>(i,j) = color;
    }
  }
}

void createStripesImage(cv::Mat & dest, cv::Size size, int stripe_size, int number)
{
  createStripesImage(dest, size.height, size.width, stripe_size, number);
}

cv::Size getScreenResolution()
{/*
  LPRECT desktop = 0;
  const HWND hDesktop = GetDesktopWindow();

  if (GetWindowRect(hDesktop, desktop))
  {
    return cv::Size(desktop->right, desktop->bottom);
  }*/
  return cv::Size(GetSystemMetrics(SM_CXFULLSCREEN),GetSystemMetrics(SM_CYFULLSCREEN));
}
