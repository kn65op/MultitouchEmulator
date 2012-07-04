#pragma once

#include "stdafx.h"
#include <opencv2\opencv.hpp>

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
   * Classs for handling exceptions.
   */
  class Exception
  {
  public:
    /**
     * Constructor with setting message.
     * @param message Message to be stored.
     * @param critical Information if error is critical.
     */
    Exception(std::string message, bool critical)
    {
      mes = message;
    }
    /**
     * Operator with allow to read message.
     * @return Message stored in exception as std::string.
     */
    std::string operator()()
    {
      return mes;
    }
    /**
     * Function returns information if error is critical.
     * @return true if error is critical, false otherwise.
     */
    bool isCritical()
    {
      return critical;
    }
  private:
    std::string mes;
    bool critical;
  };

  /** 
   * Constructor makes all needed initialization.
   */
  ApplicationController(void);
  /**
   * Default destructor.
   */
  ~ApplicationController(void);

  void init();

  /**
   * Function which take care about detecting screen.
   */
  void detectScreen();

  /**
   * Function which take care about searching for devices.
   */
  void searchingForDevices() throw(Exception);

  /**
   * Function which take care about transmission.
   */
  void transmission();

  void end();

private:
  //Homography
  Homography hom;
  //Screen
  ScreenShape ss;
  //Key
  Key key;
  //Camera
  cv::VideoCapture cap;
  //Devices
  Devices devices;
  //screen resolution
  cv::Size resolution;

  //structuring elements to use
  //structuring element ellipse 9x9
  cv::Mat strele9x9;
  //structuring element ellipse 11x11
  cv::Mat strele11x11;
  //structuring element ellipse 13x13
  cv::Mat strele13x13;
  //structuring element ellipse 3x3
  cv::Mat strele3x3;
  
};

