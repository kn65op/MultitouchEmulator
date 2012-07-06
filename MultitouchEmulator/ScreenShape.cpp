#include "StdAfx.h"
#include "ScreenShape.h"

#include <math.h>
#include <algorithm>


ScreenShape::ScreenShape(void)
{
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

  //looking for closest points to corners from image corners

  unsigned int n;
  n = std::min_element(dist00.begin(), dist00.end()) - dist00.begin();
  if (n < dist00.size())
  {
    corners.push_back(cv::Point(screen[n].x, screen[n].y));
  }
  
  n = std::min_element(distm0.begin(), distm0.end()) - distm0.begin();
  if (n < dist00.size())
  {
    corners.push_back(cv::Point(screen[n].x, screen[n].y));
  }

  n = std::min_element(dist0m.begin(), dist0m.end()) - dist0m.begin();
  if (n < dist00.size())
  {
    corners.push_back(cv::Point(screen[n].x, screen[n].y));
  }

  n = std::min_element(distmm.begin(), distmm.end()) - distmm.begin();
  if (n < dist00.size())
  {
    corners.push_back(cv::Point(screen[n].x, screen[n].y));
  }
}

std::vector<cv::Point> ScreenShape::getCorners() const
{
  return corners;
}

std::vector<cv::Point> ScreenShape::getScreen() const
{
  return screen;
}

void ScreenShape::prepare()
{
  corners.clear();
}