#pragma once

#include "stdafx.h"
#include <opencv2\opencv.hpp>

#include "ImageOperations.h"
#include "Homography.h"
#include "ScreenShape.h"
#include "Devices.h"
#include "Key.h"

/**
 * Class responsible for controlling application flow.
 */
class ApplicationController
{
public:
  /** 
   * Constructor makes all needed initialization.
   */
  ApplicationController(void);
  /**
   * Default destructor.
   */
  ~ApplicationController(void);
private:
  //Homography
  Homography hom;
  //Screen
  ScreenShape ss;
  //Key
  Key key;
  cv::VideoCapture cap;
};

