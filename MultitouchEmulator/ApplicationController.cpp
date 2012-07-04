#include "StdAfx.h"
#include "ApplicationController.h"

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

  //showing white screen to find screen
  imshow("white", hom.getGUIDetectScreen());
  showImageWithoutFrame(L"white", resolution.width, resolution.height);


}


ApplicationController::~ApplicationController(void)
{
}
