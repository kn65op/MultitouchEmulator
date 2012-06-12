#include "StdAfx.h"
#include "ScreenShape.h"

#include <math.h>
#include <algorithm>


ScreenShape::ScreenShape(void)
{
  thres = 127;
}


ScreenShape::~ScreenShape(void)
{
}

bool ScreenShape::detectScreen(cv::Mat & image)
{
  screen.clear();
  cv::Size size = image.size();
  std::vector<cv::Point> points;
  for (int i=0; i<size.width; ++i)
  {
    for (int j=0; j<size.height; ++j)
    {
      if (image.at<uchar>(j,i) == 255)
      {
        points.push_back(cv::Point(i,j));
      }
    }
  }
  if (points.empty())
  {
    return false;
  }
  cv::convexHull(cv::Mat(points), screen);
  return true;
}

void ScreenShape::printScreen(cv::Mat & image)
{
  //cv::Point2f points[4];
  //screen.points(points);
  int size = screen.size();
  for (int i=0; i<size; ++i)
  {
    cv::line(image, screen[i], screen[(i + 1) % size], cv::Scalar(255,0,0), 10, CV_AA);
  }
}

std::vector<double> ScreenShape::getSize() const
{
  std::vector<double> tmp;
  int size = screen.size();
  if (!size)
  {
    return std::vector<double> ();
  }
  for (int i=0; i < size; ++i)
  {
    tmp.push_back(sqrt(pow(static_cast<double>(screen[(i+1)%size].x - screen[i].x), 2) + pow(static_cast<double>(screen[(i+1)%size].y - screen[i].y), 2)));
  }
  std::sort(tmp.begin(), tmp.end());
  std::unique(tmp.begin(), tmp.end());
  return std::vector<double>(tmp.end() - 4, tmp.end());
}

void ScreenShape::findScreenAtBinaryImage(cv::Mat & bin)
{
  detectScreen(bin);

  cv::Size bsize = bin.size();

  int size = screen.size();
  std::vector<double> dist00(size);
  std::vector<double> distm0(size);
  std::vector<double> dist0m(size);
  std::vector<double> distmm(size);

  //calculating distances to corners
  for (int i=0; i<size; ++i)
  {
    dist00[i] = sqrt(pow((double)screen[i].x, 2.0) + pow((double)screen[i].y, 2.0));
    dist0m[i] = sqrt(pow((double)screen[i].x, 2.0) + pow(bsize.height - (double)screen[i].y, 2.0));
    distm0[i] = sqrt(pow(bsize.width - (double)screen[i].x, 2.0) + pow((double)screen[i].y, 2.0));
    distmm[i] = sqrt(pow(bsize.width - (double)screen[i].x, 2.0) + pow(bsize.height - (double)screen[i].y, 2.0));
  }
  
  corners.clear();

  //looking for closest points

  int n;
  n = std::min_element(dist00.begin(), dist00.end()) - dist00.begin();
  corners.push_back(cv::Point(screen[n].x, screen[n].y));
  
  n = std::min_element(distm0.begin(), distm0.end()) - distm0.begin();
  corners.push_back(cv::Point(screen[n].x, screen[n].y));

  n = std::min_element(dist0m.begin(), dist0m.end()) - dist0m.begin();
  corners.push_back(cv::Point(screen[n].x, screen[n].y));

  n = std::min_element(distmm.begin(), distmm.end()) - distmm.begin();
  corners.push_back(cv::Point(screen[n].x, screen[n].y));

  /*
  int size = screen.size();
  std::vector<double> angles(size);
  std::vector<double> diffs(size);

  //calculating angles of lines
  for (int i=0; i<size; ++i)
  {
    if (screen[i].y - screen[(i + 1) % size].y)
    {
      angles[i] = fabs((double)(screen[i].x - screen[(i + 1) % size].x ) / (double)(screen[i].y - screen[(i + 1) % size].y));
    }
    else
    {
      angles[i] = 300;
    }
  }
  
  //calculating differences between angles
  for (int i=1; i<size + 1; ++i)
  {
    diffs[i - 1] = fabs(angles[i%size] - angles[i-1]);
  }

  corners.clear();
  for (int i=0; i < 4; ++i)
  {
    std::vector<double>::iterator it = std::max_element(diffs.begin(), diffs.end());
    int maxit = it - diffs.begin();
    corners.push_back(cv::Point(screen[maxit].x, screen[maxit].y));
    *it = 0;
  }*/


  /*
  detectScreen(bin);

  int size = screen.size();

  for (int i=0; i<size; ++i)
  {
    if (screen[i].x > max_xx)
    {
      max_xx = screen[i].x;
      max_xy = screen[i].y;
    }
    if (screen[i].y > max_yy)
    {
      max_yx = screen[i].x;
      max_yy = screen[i].y;
    }
    if (screen[i].x < min_xx)
    {
      min_xx = screen[i].x;
      min_xy = screen[i].y;
    }
    if (screen[i].x < min_yy)
    {
      min_yx = screen[i].x;
      min_yy = screen[i].y;
    }
  }

  corners.clear();
  corners.push_back(cv::Point(max_xx, max_xy));
  corners.push_back(cv::Point(max_yx, max_yy));
  corners.push_back(cv::Point(min_xx, min_xy));
  corners.push_back(cv::Point(min_yx, min_yy));
  */
}

std::vector<cv::Point> ScreenShape::getCorners() const
{
  return corners;
}

std::vector<cv::Point> ScreenShape::getScreen() const
{
  return screen;
}