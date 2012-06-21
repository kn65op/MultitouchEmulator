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
  //Mat generated(100,100, CV_8UC1);

  //namedWindow("gray",2);

  //cv::Mat real(4,1, CV_32FC2);
  //real.at<cv::Point2f>(0,0) = cv::Point2f(0,0);
  //real.at<cv::Point2f>(1,0) = cv::Point2f(0,100);
  //real.at<cv::Point2f>(2,0) = cv::Point2f(100,0);
  //real.at<cv::Point2f>(3,0) = cv::Point2f(100,100);

  //cv::Mat image(4,1, CV_32FC2);

  //image.at<cv::Point2f>(0,0) = cv::Point2f(50,50);
  //image.at<cv::Point2f>(1,0) = cv::Point2f(50,300);
  //image.at<cv::Point2f>(2,0) = cv::Point2f(200,50);
  //image.at<cv::Point2f>(3,0) = cv::Point2f(200,300);

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

//  cv::Mat white;
  //createWhiteImage(white, resolution);

  imshow("white", hom.getGUIDetectScreen());
  showImageWithoutFrame(L"white", resolution.width, resolution.height);

  int number = 1;
  
  //searching for screen
  while (true)
  {
    cap >> frame;
    cvtColor(frame, gray, CV_RGB2GRAY);
    threshold(gray, binary, thres, 255, CV_THRESH_OTSU);
    //imshow("bin", binary);

    erode(binary, binary, strel);
    dilate(binary, binary, strel);

    dilate(binary, binary, strel);
    erode(binary, binary, strel);

    //imshow("ost", binary);

    ss.findScreenAtBinaryImage(binary);
    std::vector<cv::Point> cor = ss.getCorners();

    std::vector<cv::Point>::iterator begin, end;
    /*      std::vector<cv::Point> scre = ss.getScreen();
    end = scre.end();
    for (begin = scre.begin(); begin != end; ++begin)
    {
    //std::cout << begin->x << " " <<begin->y << " ! ";
    circle(gray, *begin, 10, cv::Scalar(0,255,0), 10);
    }*/

    end = cor.end();
    for (begin = cor.begin(); begin != end; ++begin)
    {
      //std::cout << begin->x << " " <<begin->y << " ! ";
      circle(gray, *begin, 10, cv::Scalar(255,0,0), 10);
    }

    //std::cout << "\n";
    imshow("gray",gray);
    showImageWithoutFrame(L"gray", resolution.width / 3, resolution.height / 3, gray.cols, gray.rows);

    if(waitKey(30) >= 0)
    {
      break;
    }
  }

  destroyAllWindows();

  //set corners as points on image to transformation
  cv::Mat image(4,1, CV_32FC2);
  std::vector<cv::Point> cor = ss.getCorners();

  std::vector<cv::Point>::iterator begin, end;
  end = cor.end();
  int i = 0;
  for (begin = cor.begin(); begin != end; ++begin)
  {
    image.at<cv::Point2f>(i++,0) = cv::Point2f((float)(begin->y), (float)(begin->x));
  }

  /*/
  //Manual selecting points 
  //*
  cap >> frame;
  cvtColor(frame, gray, CV_RGB2GRAY);
  imshow("gray", gray);
  cv::setMouseCallback("gray", getPointsFromImage, &hom);

  while(true)
  {
  cap >> frame;
  cvtColor(frame, gray, CV_RGB2GRAY);
  imshow("gray", gray);
  if(waitKey(30) >= 0 || hom.isPointsSet())
  {
  break;
  }
  }

  cv::setMouseCallback("gray", NULL, 0);
  //*/


  //run Homography
  //hom.setGeneratedImageSize(1000,700);
  hom.runHomography(image);

  cv::Mat generated, to_show;
  cv::Mat objects = cv::Mat::zeros(frame.size(), CV_8UC1);

  strel = cv::getStructuringElement(MORPH_ELLIPSE, cv::Size(11,11));
  cv::Mat strel_big = cv::getStructuringElement(MORPH_ELLIPSE, cv::Size(13,13));
  cv::Mat strel_small = cv::getStructuringElement(MORPH_ELLIPSE, cv::Size(3,3));

  cv::Mat hsv_all, bin1, bin2;
  cv::Mat hsv[3];
  /*for (int i=0; i < 3; ++i)
  {
  rgb[i] = cv::Mat(frame.rows, frame.cols, CV_64FC1);
  }/
  int fromto[] = {0,0,1,1,2,2};*/

  Devices devices;

  cv::Mat last_gray;

	cv::Mat pattern;
	generatePattern(pattern);

  cap >> frame; // get a new frame from camera
  cvtColor(frame, gray, CV_RGB2GRAY);

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

    /*if (isSameImage (last_gray, gray))
    {
      continue;
    }*/

    //inRange(hsv_all, cv::Scalar(10, 80, 80), cv::Scalar(30,150,255), bin1);
    //inRange(hsv_all, cv::Scalar(10, 70, 20), cv::Scalar(50,180,80), bin2);

    //split(frame, hsv);

    //threshold(hsv[0], bin1, 30, 255, THRESH_BINARY_INV);
    //threshold(hsv[0], bin2, 330, 255, THRESH_BINARY);
    
    //bitwise_or(bin1, bin2, binary);

    inRange(hsv_all, cv::Scalar(15, 0, 0), cv::Scalar(40,255,255), binary);

    //inRange(hsv_all, cv::Scalar(20, 160, 110), cv::Scalar(40,240,150), bin1); //no light
    //inRange(hsv_all, cv::Scalar(25, 100, 120), cv::Scalar(40,200,200), bin2); //light
//    bitwise_or(bin1, bin2, binary);
    
    imwrite("bin.bmp", binary);

    negation(binary);
    imwrite("bin.bmp", binary);
    cv::dilate(binary, binary, strel_small);
    imwrite("bin.bmp", binary);
    cv::erode(binary, binary, strel_small);
    imwrite("bin.bmp", binary);


    imshow("bin", binary);
    //imshow("g", hsv[1]);
    //imshow("b", hsv[2]);

    //threshold(gray, binary, thres, 255, CV_THRESH_OTSU);
    //imshow("bin", binary);

    //for (int i=50; i<200; ++i)
    //{
    //  for (int j=50; j<300; ++j)
    //  {
    //    cv::Point tmp = hom.getRealPoint(i, j);
    //    generated.at<uchar>(tmp.x, tmp.y) = gray.at<uchar>(i,j);
    //  }
    //}
    
    imwrite("to_show.bmp", binary);
    generated = hom.processImage(binary);
    imwrite("to_show.bmp", generated);
    cv::erode(generated, to_show, strel);
    imwrite("to_show.bmp", to_show);
    cv::dilate(to_show, to_show, strel_big  );
    imwrite("to_show.bmp", to_show);
    
    //cv::dilate(to_show, to_show, strel);
    //cv::erode(to_show, to_show, strel);

    imwrite("to_show.bmp", to_show);
    
    indexImageBlack(to_show, objects, devices);


    //cv::equalizeHist(objects, objects);
    //imshow("objects", objects);
    //imwrite("objects.jpg", objects);

	//		imshow("pattern", pattern);
		//	showImageWithoutFrame(L"pattern", 500, 500, pattern.cols, pattern.rows);

    if (number ++ > 5)
    {
      //imshow("generated", to_show);
      imshow("generated", hom.getGUIDetectDevice(devices));
      showImageWithoutFrame(L"generated", to_show.cols, to_show.rows);

    }    
    if(waitKey(30) >= 0)
    {
      break;
    }
  }

  //setting transmission
  Key key;
  //std::cout << devices.size() << ": ";
  key.setNumberOfDevices(devices.size());
  key.generateMainKey(128);
  key.setHashLength(128);

    
  devices.processToTransmition(key);

  //transmission

  while(true)
  {
    Sleep(500);
    imshow("generated", hom.getGUITransmission(devices));
    showImageWithoutFrame(L"generated", resolution.width, resolution.height);

    if(waitKey(30) >= 0)
    {
      break;
    }
  }
#endif

#ifdef CALIB
  int numBoards = 10;
  int numCornersHor = 8;
  int numCornersVer = 5;
  int numSquares = numCornersHor * numCornersVer;
  Size board_sz = Size(numCornersHor, numCornersVer);
  VideoCapture capture = VideoCapture(0);

  vector<vector<Point3f>> object_points;
  vector<vector<Point2f>> image_points;
  vector<Point2f> corners;
  int successes=0;
  Mat image;
  Mat gray_image;
  capture >> image;
  vector<Point3f> obj;
  for(int j=0;j<numSquares;j++)
    obj.push_back(Point3f(j/numCornersHor, j%numCornersHor, 0.0f));
  while(successes<numBoards)
  {
    cvtColor(image, gray_image, CV_BGR2GRAY);
    bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

    if(found)
    {
      std::cout << "OK\n";
      cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
      drawChessboardCorners(gray_image, board_sz, corners, found);
    }
    imshow("win1", image);
    imshow("win2", gray_image);

    capture >> image;

    int key = waitKey(1);
    if(key==27)
      return 0;

    if(key==' ' && found!=0)
    {
      image_points.push_back(corners);
      object_points.push_back(obj);
      printf("Snap stored!\n");

      successes++;

      if(successes>=numBoards)
        break;
    }
  }
  Mat intrinsic = Mat(3, 3, CV_32FC1);
  Mat distCoeffs;
  vector<Mat> rvecs;
  vector<Mat> tvecs;
  intrinsic.ptr<float>(0)[0] = 1;
  intrinsic.ptr<float>(1)[1] = 1;
  calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
  Mat imageUndistorted;
  while(1)
  {
    capture >> image;
    undistort(image, imageUndistorted, intrinsic, distCoeffs);

    imshow("win1", image);
    imshow("win2", imageUndistorted);

    waitKey(1);
  }
  capture.release();

#endif CALIB
#ifdef CAME
  /* cv::Mat im1 = cv::imread("stripes1ob.png");
  cv::Mat im2 = cv::imread("stripes2ob.png");

  cv::Mat gim1, gim2;
  cv::cvtColor(im1, gim1, CV_RGB2GRAY);
  cv::cvtColor(im2, gim2, CV_RGB2GRAY);

  cv::Mat sub;
  subtractAbs(gim1, gim2, sub);

  imshow("sub", sub);*/

  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
    return -1;

  bool a = true;

  cv::Mat last_frame;
  cv::Mat act_frame;
  cv::Mat act_frame_rgb;
  cv::Mat difference_frame;

  cap >> act_frame_rgb;

  cvtColor(act_frame_rgb, act_frame, CV_RGB2GRAY);

  while(waitKey(30) < 0)
  {
    //TODO degub only
    Sleep(300);

    //capture frame and change to gray scale
    cap >> act_frame_rgb;

    act_frame.copyTo(last_frame);
    cvtColor(act_frame_rgb, act_frame, CV_RGB2GRAY);

    //difference
    subtractAbs(last_frame, act_frame, difference_frame);
    //imshow("Difference", act_frame);

    //imshow("Difference", difference_frame);

    cv::Mat tmp;
    cv::Size desktop = getScreenResolution();
    //createStripesImage(tmp, desktop, 50, a);
    createStripesImage(tmp, 1000, 1000, 50, a);
    imshow("Background", tmp);

    imshow("Difference", difference_frame);
    imshow("Camera", act_frame_rgb);



    //change stripes
    a = !a;

  }
#endif

#ifdef CAM
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
    return -1;

  double thres = 127;

  ScreenShape ss;

  Mat frame, gray;
  namedWindow("frame",1);
  namedWindow("gray",2);
  namedWindow("bin",3);
  for(;;)
  {

    cap >> frame; // get a new frame from camera
    cvtColor(frame, gray, CV_RGB2GRAY);
    imshow("gray", gray);
    threshold(gray, gray, thres, 255, CV_THRESH_BINARY);
    imshow("bin", gray);
    ss.detectScreen(gray);
    //  ss.printScreen(frame);
    imshow("frame", frame);
    std::vector<double> bord = ss.getSize();
    for (std::vector<double>::iterator it = bord.begin(); it != bord.end(); ++it)
    {
      std::cout << *it << " ";
    }
    std::cout << "\n";
    if(waitKey(30) >= 0)
    {
      break;
    }

  }

  // the camera will be deinitialized automatically in VideoCapture destructor'
#endif 
  return 0;
}

