#include "StdAfx.h"
#include "Homography.h"

#include <cmath>
#include <sstream>

#include "ImageOperations.h"

Homography::Homography(void)
{
  //starting parameters
  points_setted = false;
  colorGUI = cv::Scalar(100,255,100);
  color_line = cv::Scalar(0,0,0);
  color_detect_screen = cv::Scalar(255,255,255);
  color_black = cv::Scalar(0,0,0);
  color_white = cv::Scalar(255,255,255);
  
  //parameters defiding the scene
  //device height in cm (double)
  h1 = 4;
  //camera height in cm (double)
  h = 72;
  // camera position in x dimension in pixels (double)
  camera_pos_x = 300;
  // camera position in y dimension in pixels (double)
  camera_pos_y = -200;
  // probability of making mistakes during transmission (double)
  mistake_posibility = 0;
  
  //calculating scale
  scale = 1 - h1 / h;

  //random engine for mistakes
  gen_mistakes = new std::mt19937(rd_mistakes());
  dist_mistakes = mistake_posibility ?  new std::uniform_int_distribution<>(0, static_cast<int>(1 / mistake_posibility - 1)) : new std::uniform_int_distribution<>(1,1);

  //reset
  reset();
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
  double x1 = x;// * scale;
  double y1 = y;// * scale;
  cv::Mat image_point = (cv::Mat_<double>(3,1) << x1, y1, 1);
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
  level = (int)(generated_x * 0.1) - 1;
}

void Homography::setGeneratedImageSize(cv::Size size)
{
  setGeneratedImageSize(size.height, size.width);
  
  int level = (int)(generated_x * 0.1);
  cv::line(GUI, cv::Point(0, level), cv::Point(generated_y, level), color_line, 1);
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
      //changing points from image to reality
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
  //This is done, beacuse of performance.
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
  reset();

  image_points.copyTo(this->image_points);

  runHomography();
}

void Homography::setROI(cv::Mat & frame) const
{
  frame = cv::Mat(frame, cv::Rect(min_y, min_x, max_y - min_y, max_x - min_x));
}

cv::Mat & Homography::getGUIDetectDevice(Devices & devs)
{
  static int last_no_of_devices = 0;
  clearGUI();
  
  //text
  std::stringstream text;
  if (devs.size() == last_no_of_devices)
  {
    text << devs.size();
    text << " devices found. Press any key to initialize transmission";
  }
  else
  {
    last_no_of_devices = devs.size();
    text << "Waiting for still scene";
  }
  cv::putText(GUI, text.str(), cv::Point((int)(generated_y * 0.05), (int)(generated_x * 0.05)), CV_FONT_HERSHEY_SIMPLEX, 1, color_line);
  return GUI;
}

cv::Mat & Homography::getGUIStillScreen()
{
  clearGUI();

  //text
  std::stringstream text;
  text << "Waiting for still scene";
  cv::putText(GUI, text.str(), cv::Point((int)(generated_y * 0.05), (int)(generated_x * 0.05)), CV_FONT_HERSHEY_SIMPLEX, 1, color_line);
  return GUI;
}

cv::Mat & Homography::getGUIDetectScreen()
{
  clearGUI();
  setGUIColor(color_detect_screen);
  cv::putText(GUI, "Press any key when circles points to screen corners", cv::Point((int)(generated_y * 0.05), (int)(generated_x * 0.05)), CV_FONT_HERSHEY_SIMPLEX, 1, color_line);
  return GUI;
}

void Homography::clearGUI()
{
  cv::rectangle(GUI, cv::Rect(0,0,generated_x, level), colorGUI, CV_FILLED);
}

cv::Mat & Homography::getGUITransmission(Devices & devs)
{
  clearGUI();
  
  //showing progress
  double tmp = static_cast<double>(++transmission_progress) / static_cast<double>(transmission_length) * 100.0;
  tmp = tmp > 100 ? 100 : tmp;

  end = tmp == 100;

  //blinking rectangles
  Devices::iterator it, end;
  end = devs.getEnd();
  for (it = devs.getBegin(); it != end; ++it)
  {
    //second version of making stupid of atacker
    //it->second->showRandomBlinkAround(GUI);
    
    if (tmp < 100)
    {
      it->second->showNoiseAround(GUI);
    }
    //mistakes
    if ((*dist_mistakes)(*gen_mistakes))
    {
      it->second->showNextBit(GUI);
    }
    else
    {
      it->second->showWrongBit(GUI);
    }
  }
  std::stringstream ss;
  
  //text
  ss << "Transmission in progress: " << tmp  << "%.";

  //progress bar
  cv::putText(GUI, ss.str(), cv::Point((int)(generated_y * 0.05), (int)(generated_x * 0.05)), CV_FONT_HERSHEY_SIMPLEX, 1, color_line);
  cv::rectangle(GUI, cv::Rect(static_cast<int>(generated_y * 0.5), static_cast<int>(generated_x * 0.025), static_cast<int>(generated_y * 0.4), static_cast<int>(generated_x * 0.05)), color_black, CV_FILLED);
  cv::rectangle(GUI, cv::Rect(static_cast<int>(generated_y * 0.501), static_cast<int>(generated_x * 0.026), static_cast<int>(tmp / 100 * generated_y * 0.399), static_cast<int>(generated_x * 0.048)), color_white, CV_FILLED);
  return GUI;
}

void Homography::setGUIColor(cv::Scalar & scalar)
{
  GUI = cv::Mat(generated_x, generated_y, CV_8UC3, scalar);
}

void Homography::randomBlink()
{
  cv::Size size = GUI.size();
  int level2 = level + 1;

  //create random engine
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, 255);

  for (int i=level2; i<size.height; ++i)
  {
    for (int j=0; j<size.width; ++j)
    {
      GUI.at<cv::Vec3b>(i,j) = cv::Vec3b(dist(gen), dist(gen), dist(gen));
    }
  }
}

double Homography::getShiftX() const
{
  return scale;
}

double Homography::getShiftY() const
{
  return scale;
}

double Homography::getCameraX() const
{
  return camera_pos_x;
}

double Homography::getCameraY() const
{
  return camera_pos_y;
}

cv::Mat & Homography::getGUICameraPosition()
{
  setGUIColor(cv::Scalar(255,255,255));
  makeCheck(GUI);
  return GUI;
}

cv::Mat & Homography::getGUIEnd()
{
  setGUIColor(color_black);
  cv::putText(GUI, "Transmission finished, press any key to exit.", cv::Point((int)(generated_y * 0.05), (int)(generated_x * 0.05)), CV_FONT_HERSHEY_SIMPLEX, 1, color_white);
  return GUI;
}

bool Homography::isEnd() const
{
  return end;
}

void Homography::prepareDeviceRecognition()
{
  setGUIColor(colorGUI);
}

void Homography::prepareTransmission(Devices & devs)
{
  setGUIColor(colorGUI);
  transmission_progress = -1;
  transmission_length = devs.getMaxKeyLength();
}

cv::Mat & Homography::getGUIBlackScreen()
{
  setGUIColor(color_black);
  cv::putText(GUI, "Prepare devices and press any key to start transmission.", cv::Point((int)(generated_y * 0.05), (int)(generated_x * 0.05)), CV_FONT_HERSHEY_SIMPLEX, 1, color_white);
  return GUI;
}

void Homography::reset()
{
  min_x = min_y = 4000;
  max_x = max_y = 0;
}

void Homography::setParameters(Parameters & par)
{
  //TODO: dopisaæ
}