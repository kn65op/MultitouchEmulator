#include "StdAfx.h"
#include "ApplicationController.h"

#include <vector>

#include "ImageOperations.h"


ApplicationController::ApplicationController(void) throw (ApplicationController::Exception) : cap(0)
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
  strele9x9 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9,9));

}


ApplicationController::~ApplicationController(void)
{
}

void ApplicationController::detectScreen()
{
  cv::Mat frame, binary, gray;

  //showing white screen to find screen
  cv::imshow("white", hom.getGUIDetectScreen());
  showImageWithoutFrame(L"white", resolution.width, resolution.height);
  
  //searching for screen
  while (true)
  {
    cap >> frame;
    cv::cvtColor(frame, gray, CV_RGB2GRAY);
    cv::threshold(gray, binary, 0, 255, CV_THRESH_OTSU);
    
    cv::erode(binary, binary, strele9x9);
    cv::dilate(binary, binary, strele9x9);

    cv::dilate(binary, binary, strele9x9);
    cv::erode(binary, binary, strele9x9);

    ss.findScreenAtBinaryImage(binary);
    std::vector<cv::Point> cor = ss.getCorners();

    std::vector<cv::Point>::iterator begin, end;
    
    end = cor.end();
    int qwe = 5;
    for (begin = cor.begin(); begin != end; ++begin)
    {
      cv::circle(gray, *begin, 10, cv::Scalar(255,0,0), qwe);
      qwe += 5;
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