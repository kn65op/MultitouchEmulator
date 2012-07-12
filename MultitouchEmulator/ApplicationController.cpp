#include "StdAfx.h"
#include "ApplicationController.h"

#include <vector>

#include "ImageOperations.h"
#include "EndWindow.h"


ApplicationController::ApplicationController(void) : cap(0)
{
  time = 0;
  end = false;
  show_detect_screen = show_starting_dialog = show_device_recognition = true; 
}

void ApplicationController::init()
{
  if (!cap.isOpened())
  {
    throw ApplicationController::Exception("Camera could not be opened", true);
  }
  resolution = getScreenResolution();

  //setting homography (images size);
  cv::Mat frame, gray;
  cap >> frame;
  cvtColor(frame, gray, CV_RGB2GRAY);
  hom.setImageSize(gray.size());
  hom.setGeneratedImageSize(resolution);

  //generating structuring elements
  strele3x3 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3));
  strele9x9 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9,9));
  strele11x11 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(11,11));
  strele13x13 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(13,13));
}


ApplicationController::~ApplicationController(void)
{
}

void ApplicationController::detectScreen()
{
  if (!show_detect_screen)
  {
    return;
  }
  //setting detecting
  cv::Mat frame, binary, gray;

  //showing white screen to find screen
  cv::imshow("white", hom.getGUIDetectScreen());
  showImageWithoutFrame(L"white", resolution.width, resolution.height);
  
  //searching for screen
  while (true)
  {
    cap >> frame;
    cv::cvtColor(frame, gray, CV_RGB2GRAY);
    cv::threshold(gray, binary, 0/*doesn't matter*/, 255, CV_THRESH_OTSU);
    
    cv::erode(binary, binary, strele9x9);
    cv::dilate(binary, binary, strele9x9);

    cv::dilate(binary, binary, strele9x9);
    cv::erode(binary, binary, strele9x9);

    ss.findScreenAtBinaryImage(binary);
    std::vector<cv::Point> cor = ss.getCorners();

    std::vector<cv::Point>::iterator begin, end;
    
    end = cor.end();
    for (begin = cor.begin(); begin != end; ++begin)
    {
      cv::circle(gray, *begin, 10, cv::Scalar(255,0,0), 7);
    }

    imshow("gray",gray);
    showImageWithoutFrame(L"gray", resolution.width / 3, resolution.height / 3, gray.cols, gray.rows);

    if(cv::waitKey(30) >= 0)
    {
      break;
    }
  }

  cv::destroyAllWindows();

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
}

void ApplicationController::searchingForDevices()
{
  if (!show_device_recognition)
  {
    return;
  }

  hom.prepareDeviceRecognition();

  //setting searching
  cv::Mat frame, hsv_all, generated, to_show, objects, binary;
  int number = 1;

  //searching
  while(true)
  {
    cap >> frame; // get a new frame from camera
    cv::cvtColor(frame, hsv_all, CV_BGR2HSV);

    //imshow("gray", gray);
    
    cv::inRange(hsv_all, cv::Scalar(15, 0, 30), cv::Scalar(40,255,255), binary);

    negation(binary);
    cv::dilate(binary, binary, strele3x3);
    cv::erode(binary, binary, strele3x3);

    //cv::imshow("bin", binary);
    
    generated = hom.processImage(binary);
    cv::erode(generated, to_show, strele11x11);
    cv::dilate(to_show, to_show, strele13x13);
    
    if(number++ > 5)
    {
      indexImageBlack(to_show, objects, devices);
      cv::imshow("generated", hom.getGUIDetectDevice(devices));
      showImageWithoutFrame(L"generated", to_show.cols, to_show.rows);
    }
    else
    {
      cv::imshow("generated", hom.getGUIStillScreen());
      showImageWithoutFrame(L"generated", to_show.cols, to_show.rows);
    }

    if(cv::waitKey(30) >= 0)
    {
      break;
    }
  }

  devices.shiftDevices(hom);
}

void ApplicationController::transmission()
{

  //transmission
  while(true)
  {
    Sleep(time);

    cv::imshow("generated", hom.getGUITransmission(devices));
    showImageWithoutFrame(L"generated", resolution.width, resolution.height);

    if(cv::waitKey(30) >= 0 || hom.isEnd())
    {
      break;
    }
  }
}

void ApplicationController::endingScreen()
{
   while (true)
  {
    cv::imshow("generated", hom.getGUIEnd());
    showImageWithoutFrame(L"generated", resolution.width, resolution.height);

    if(cv::waitKey(30) >= 0)
    {
      break;
    }
  }
}

void ApplicationController::showCheck()
{
  //setting camera position
  cv::Mat & check = hom.getGUICameraPosition();
  while (true)
  {
    cv::imshow("Check", check);
    showImageWithoutFrame(L"Check", resolution);
    if(cv::waitKey(30) >= 0)
    {
      break;
    }
  }

  cv::destroyAllWindows();
}

void ApplicationController::processEndingDialog(int response)
{
  switch (response)
  {
  case EndWindow::DEVICE:
    show_detect_screen = show_starting_dialog = end = false;
    show_device_recognition = true;
    break;
  case EndWindow::END:
    end = true;
    break;
  case EndWindow::SCREEN:
    show_detect_screen = show_device_recognition = true;
    show_starting_dialog = end = false;
    break;
  case EndWindow::START:
    show_detect_screen = show_starting_dialog = show_device_recognition = true;
    end = false;
    break;
  case Gtk::RESPONSE_DELETE_EVENT:
    end = true;
    break;
  case EndWindow::TRANSMISSION:
    show_detect_screen = show_device_recognition = show_starting_dialog = end = false;
  }
}

bool ApplicationController::isRun() const
{
  return !end;
}
  
bool ApplicationController::isShowStartingDialog() const
{
  return show_starting_dialog;
}

void ApplicationController::prepareTransmission()
{  
  if (!devices.size())
  {
    throw Exception("No devices found", false);
  }
  while(true)
  {
    cv::imshow("generated", hom.getGUIBlackScreen());
    showImageWithoutFrame(L"generated", resolution.width, resolution.height);
    if(cv::waitKey(30) >= 0)
    {
      break;
    }
  }

  //settig transmission
  key.setNumberOfDevices(devices.size());
  //key.generateMainKey(128);
  key.generateMainKeyFourTimes(32);
  key.setHashLength(160);

  devices.processToTransmition(key, hom);

  hom.prepareTransmission(devices);
}

void ApplicationController::setParameters(Parameters & par)
{
  time = par.GetTime();
  hom.setParameters(par);
}