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



int _tmain(int argc, _TCHAR* argv[])
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
  ApplicationController ac;

  ac.detectScreen();

  VideoCapture cap(0);

  Homography hom;
  ScreenShape ss;

  cv::Size resolution;

  Mat frame, gray;

  //setting the Homography
  

  //Automatic selecting points
  Mat binary;

  Mat strel;
  

  int number = 1;
  
  
  cv::Mat generated, to_show;
  cv::Mat objects = cv::Mat::zeros(frame.size(), CV_8UC1);

  strel = cv::getStructuringElement(MORPH_ELLIPSE, cv::Size(11,11));
  cv::Mat strel_big = cv::getStructuringElement(MORPH_ELLIPSE, cv::Size(13,13));
  cv::Mat strel_small = cv::getStructuringElement(MORPH_ELLIPSE, cv::Size(3,3));

  cv::Mat hsv_all, bin1, bin2;
  cv::Mat hsv[3];

  Devices devices;

  cv::Mat last_gray;

	cv::Mat pattern;
	generatePattern(pattern);

  cap >> frame; // get a new frame from camera
  cvtColor(frame, gray, CV_RGB2GRAY);

  //setting camera position
  /*
  cv::Mat & check = hom.getGUICameraPosition();
  while (true)
  {
    imshow("Check", check);
    showImageWithoutFrame(L"Check", resolution);
        if(waitKey(30) >= 0)
    {
      break;
    }
  }

  double ttttt;
  std::cin >> ttttt;

  destroyAllWindows();*/

  //searching for devices
  while(true)
  {
    std::cout << number << "\n";
    gray.copyTo(last_gray);

    cap >> frame; // get a new frame from camera
    cvtColor(frame, gray, CV_RGB2GRAY);
    cvtColor(frame, hsv_all, CV_BGR2HSV);
    split(hsv_all, hsv);

    imshow("gray", gray);

    inRange(hsv_all, cv::Scalar(15, 0, 0), cv::Scalar(40,255,255), binary);

    negation(binary);
    cv::dilate(binary, binary, strel_small);
    cv::erode(binary, binary, strel_small);

    imshow("bin", binary);
    
    generated = hom.processImage(binary);
    cv::erode(generated, to_show, strel);
    cv::dilate(to_show, to_show, strel_big  );

    
    if(number > 7)
    {
      indexImageBlack(to_show, objects, devices);
    }

    if (number ++ > 3)
    {
      imshow("generated", hom.getGUIDetectDevice(devices));
      showImageWithoutFrame(L"generated", to_show.cols, to_show.rows);
    }    
    if(waitKey(30) >= 0)
    {
      break;
    }
  }

  if (!devices.size())
  {
    std::cout << "No devices, exit\n";
    return 0;
  }

  //setting transmission
  Key key;
  key.setNumberOfDevices(devices.size());
  key.generateMainKey(128);
  key.setHashLength(128);

  devices.processToTransmition(key, hom);
  

  //transmission

  while(true)
  {
    //Sleep(100); // DEBUG to see what happening

    imshow("generated", hom.getGUITransmission(devices));
    showImageWithoutFrame(L"generated", resolution.width, resolution.height);

    if(waitKey(30) >= 0 || hom.isEnd())
    {
      break;
    }
  }

  while (true)
  {
    imshow("generated", hom.getGUIEnd());
    showImageWithoutFrame(L"generated", resolution.width, resolution.height);

    if(waitKey(30) >= 0)
    {
      break;
    }
  }
#endif
  return 0;
}

