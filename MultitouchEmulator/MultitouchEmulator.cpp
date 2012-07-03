// MultitouchEmulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>

#include "ImageOperations.h"
#include "Homography.h"
#include "ScreenShape.h"
#include "Devices.h"
#include "Key.h"

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
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
    return -1;

  Homography hom;
  ScreenShape ss;

  cv::Size resolution = getScreenResolution();

  std::cout << resolution.width << " " << resolution.height << "\n";

  Mat frame, gray;

  //setting the Homography
  cap >> frame;
  cvtColor(frame, gray, CV_RGB2GRAY);
  hom.setImageSize(gray.size());
  hom.setGeneratedImageSize(resolution);

  //Automatic selecting points
  //*
  Mat binary;
  double thres = 140;

  Mat strel = getStructuringElement(MORPH_ELLIPSE, Size(9,9));
  
  imshow("white", hom.getGUIDetectScreen());
  showImageWithoutFrame(L"white", resolution.width, resolution.height);

  int number = 1;
  
  //searching for screen
  while (true)
  {
    cap >> frame;
    cvtColor(frame, gray, CV_RGB2GRAY);
    threshold(gray, binary, thres, 255, CV_THRESH_OTSU);
    
    erode(binary, binary, strel);
    dilate(binary, binary, strel);

    dilate(binary, binary, strel);
    erode(binary, binary, strel);

    ss.findScreenAtBinaryImage(binary);
    std::vector<cv::Point> cor = ss.getCorners();

    std::vector<cv::Point>::iterator begin, end;
    
    end = cor.end();
    int qwe = 5;
    for (begin = cor.begin(); begin != end; ++begin)
    {
      circle(gray, *begin, 10, cv::Scalar(255,0,0), qwe);
      qwe += 5;
    }

    imshow("gray",gray);
    showImageWithoutFrame(L"gray", resolution.width / 3, resolution.height / 3, gray.cols, gray.rows);

    if(waitKey(30) >= 0)
    {
      break;
    }
  }

  destroyAllWindows();

  //set corners as points on image to transformation
  cv::Mat image_points(4,1, CV_32FC2);
  std::vector<cv::Point> cor = ss.getCorners();

  std::vector<cv::Point>::iterator begin, end;
  end = cor.end();
  int i = 0;
  for (begin = cor.begin(); begin != end; ++begin)
  {
    image_points.at<cv::Point2f>(i++,0) = cv::Point2f((float)(begin->y), (float)(begin->x));
  }

  //run Homography
  hom.runHomography(image_points);

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

