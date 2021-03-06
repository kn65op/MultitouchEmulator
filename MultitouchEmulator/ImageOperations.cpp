#include "stdafx.h"

#include "ImageOperations.h"
#include <map>
#include <set>

void subtractAbs(cv::Mat & s1, cv::Mat & s2, cv::Mat & dest)
{

  cv::Size size = s1.size();
  dest = cv::Mat(size, CV_8UC1);

  for (int i=0; i<size.width; ++i)
  {
    for (int j=0; j<size.height; ++j)
    {
      //calculating absolute value of substraction of two pixels, multiply by 2 to get values from 0 to 254 (not from 0 to 127)
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
{
  //getting screen resolution
  RECT rc;
  GetWindowRect(GetDesktopWindow(), &rc);

  //change it to cv::Size
  return cv::Size(rc.right - rc.left, rc.bottom - rc.top);
}

void negation(cv::Mat & im)
{
  cv::Size size = im.size();
  for (int i=0; i<size.height; ++i)
  {
    for (int j=0; j<size.width; ++j)
    {
      if (im.at<uchar>(i,j))
      {
        im.at<uchar>(i,j) = 0;
      }
      else
      {
        im.at<uchar>(i,j) = 255;
      }
    }
  }
}

void indexImageBlack(cv::Mat & source, cv::Mat & index, Devices & devs)
{
  //This funtion implements image indexing using classical connected component labelling algorithm
  // 0 - object
  // 255 - background
  
  cv::imwrite("tmp.bmp", source);

  index = cv::Mat::zeros(source.size(), source.type());

  cv::Size size = source.size();
  --size.height;
  --size.width;
  std::map<int, int> map;
  int last_number = 1;

  std::set<int> set;
  std::set<int>::iterator sit, send;

  //indexing
  for (int i=1; i<size.height; ++i)
  {
    for (int j=1; j<size.width; ++j)
    {
      //object pixel
      if (!source.at<uchar>(i,j))
      {
        if (source.at<uchar>(i-1, j-1) + source.at<uchar>(i-1, j) + source.at<uchar>(i-1, j+1) + source.at<uchar>(i, j-1)  == 1020) // all pixels are from background
        {
          map[last_number] = last_number;
          index.at<uchar>(i,j) = last_number;
          ++last_number;
        }
        else //there is some object around
        {
          set.clear();
          if (index.at<uchar>(i-1, j-1))  set.insert(index.at<uchar>(i-1, j-1));
          if (index.at<uchar>(i-1, j)) set.insert(index.at<uchar>(i-1, j));
          if (index.at<uchar>(i-1, j+1)) set.insert(index.at<uchar>(i-1, j+1));
          if (index.at<uchar>(i, j-1)) set.insert(index.at<uchar>(i, j-1));
          if (set.size() > 1) //2 values different from 0, then we choose the lowest and change map 
          {
            send = set.end();
            for (sit = ++(set.begin()); sit != send; ++sit)
            {
              map[*sit] = *(set.begin());
            }
            index.at<uchar>(i,j) = *(set.begin());
          }
          else if (set.size() > 0)//only one value different from 0
          {
            index.at<uchar>(i,j) = *(set.begin());
          }
          else //i or j = 1;
          {
            map[last_number] = last_number;
            index.at<uchar>(i,j) = last_number;
            ++last_number;
          }
        }
      }
    }
  }

  //fix paste tab
  for (auto i=0; i < last_number; ++i)
  {
    while (map[i] != i && map[map[i]] != map[i])
    {
      map[i] = map[map[i]];
    }
  }

  int tmp;

  devs.clear();

  //using paste tab
  for (int i=0; i<size.height; ++i)
  {
    for (int j=0; j<size.width; ++j)
    {
      if (!source.at<uchar>(i,j)) //other value then 0, means that this is object
      {
        tmp = map[index.at<uchar>(i,j)];
        index.at<uchar>(i,j) = tmp; //mapping
        devs.setPixel(tmp, i, j);
      }
    }
  }

  //check found devices
  devs.processNewScene();
}

void createWhiteImage(cv::Mat & image, int x, int y)
{
  image = cv::Mat(x, y, CV_8UC1);
  for (int i = 0; i < x; ++i)
  {
    for (int j=0; j < y; ++j)
    {
      image.at<uchar>(i,j) = 255;
    }
  }
}

void createWhiteImage(cv::Mat & image, cv::Size & size)
{
  createWhiteImage(image, size.height, size.width);
}

void showImageWithoutFrame(LPCWSTR name, int x, int y, int width, int height)
{
  HWND win_handle = FindWindow(0, name);
  //Check if window exists
  if (!win_handle)
  {
    return;
  }

  // Resize
  unsigned int flags = (SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
  flags &= ~SWP_NOSIZE;
  SetWindowPos(win_handle, HWND_NOTOPMOST, x, y, width, height, flags);

  // Borderless
  SetWindowLong(win_handle, GWL_STYLE, GetWindowLong(win_handle, GWL_EXSTYLE) | WS_EX_TOPMOST);
  ShowWindow(win_handle, SW_SHOW);

}

void showImageWithoutFrame(LPCWSTR name, int width, int height)
{
  showImageWithoutFrame(name, 0,0, width, height);
}

void showImageWithoutFrame(LPCWSTR name, cv::Size size)
{
  showImageWithoutFrame(name, 0,0, size.width, size.height);
}

bool isSameImage(cv::Mat & one, cv::Mat & two)
{
  //unused
  if (one.type() != two.type())
  {
    return false;
  }
  cv::Size size = one.size();
  for (int i=0; i<size.height; ++i)
  {
    for (int j=0; j<size.width; ++j)
    {
      int diff = one.at<uchar>(i,j) - two.at<uchar>(i,j);
      if (diff > 5 || diff < -5)
      {
        return false;
      }
    }
  }
  return true;
}

void generatePattern(cv::Mat & pat)
{
	pat = cv::Mat(200, 200, CV_8UC3, cv::Scalar(83,201,166));
}

double distance(cv::Point2f & a, cv::Point2f & b)
{
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void makeCheck(cv::Mat & image)
{
  cv::Size size = image.size();
  for (int i=0; i<size.height; i = i + 100)
  {
    cv::line(image, cv::Point(0, i), cv::Point(size.width, i), cv::Scalar(0,0,0), 2);
  }
  for (int j=0; j<size.width; j = j + 100)
  {
    cv::line(image, cv::Point(j, 0), cv::Point(j, size.height), cv::Scalar(0,0,0), 2);
  }
}