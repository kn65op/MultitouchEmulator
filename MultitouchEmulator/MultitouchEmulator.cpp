// MultitouchEmulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>

#include "ImageOperations.h"
#include "Homography.h"
#include "ScreenShape.h"
#include "Devices.h"
#include "Key.h"

#include "ApplicationController.h"

#include "EndWindow.h"

using namespace cv;

#define HOMOGRAPHY
//#define JUST_PHOTO
//#define CAM

#ifdef JUST_PHOTO

void onMouse(int event, int x, int y, int flags, void* param)
{
  if (event == CV_EVENT_LBUTTONDOWN)
  {
    cv::Mat* hsv = static_cast<cv::Mat*>(param);
    cv::Mat tmp[3];
    split(*hsv, tmp);
    std::cout << (int)(tmp->at<uchar>(y,x)) << " " << ((int)tmp[1].at<uchar>(y,x)) << " " << ((int)tmp[2].at<uchar>(y,x))  << "\n";
  }
}

#endif



int _tmain(int argc, char* argv[])
{

#ifdef JUST_PHOTO
	cv::Mat pattern;
	generatePattern(pattern);

  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
    return -1;

  cv::Size res = getScreenResolution();

  cv::Mat frame, hsv_all;
  cap >> frame;
  imshow("frame", frame);
  cv::setMouseCallback("frame", onMouse, &hsv_all);
  while (true)
  {
    cap >> frame;
    cvtColor(frame, hsv_all, CV_BGR2HSV);
    imshow("frame", frame);
			//imshow("pattern", pattern);
		//	showImageWithoutFrame(L"pattern", 500, 500, pattern.cols, pattern.rows);
    if(waitKey(30) >= 0)
    {
      break;
    }
  }

  imwrite("photo.jpg", frame);
#endif

#ifdef HOMOGRAPHY

  Gtk::Main kit(&argc, &argv);

  ApplicationController ac;

  //initialization
  try
  {
    ac.init();
  }
  catch(ApplicationController::Exception e)
  {
    Glib::ustring mes = "Error during initialization: ";
    mes += e();
    if (e.isCritical())
    {
      mes += "\nError is critical, exiting\n";
      Gtk::MessageDialog dialog(mes);
      dialog.run();
      return -1;
    }
    Gtk::MessageDialog dialog(mes);
    dialog.run();
  }

  //procesing
  try
  {
    ac.detectScreen();

    ac.searchingForDevices();

    ac.transmission();
  }
  catch(ApplicationController::Exception e)
  {
    std::string mes = "Error during processing: " + e() + "\n";
    if (e.isCritical())
    {
      mes += "Error is critical, exiting\n";
      Gtk::MessageDialog dialog(mes);
      dialog.run();
      return -1;
    }
    Gtk::MessageDialog dialog(mes);
    dialog.run();
  }

  ac.end();

  EndWindow ew;
  ew.run();
 
#endif
  return 0;
}

