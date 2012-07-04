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
  ApplicationController(void) throw(Exception);
  /**
   * Default destructor.
   */
  ~ApplicationController(void);

  void detectScreen();

private:
  //Homography
  Homography hom;
  //Screen
  ScreenShape ss;
  //Key
  Key key;
  cv::VideoCapture cap;
  //screen resolution
  cv::Size resolution;

  //structuring elements to use
  //structuring element ellipse 9x9
  cv::Mat strele9x9;
  
};

