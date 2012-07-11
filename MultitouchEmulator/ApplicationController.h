#pragma once

#include "stdafx.h"
#include <opencv2\opencv.hpp>

#include "Homography.h"
#include "ScreenShape.h"
#include "Devices.h"
#include "Key.h"
#include "Parameters.h"

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
      this->critical = critical;
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
   * Constructor doesn't make all needed initialization. After creating object should be evaluated 'init' function.
   */
  ApplicationController(void);
  /**
   * Default destructor.
   */
  ~ApplicationController(void);

  /**
   * Function initializes object.
   */
  void init();

  /**
   * Function which take care about detecting screen.
   */
  void detectScreen();

  /**
   * Function which take care about searching for devices.
   */
  void searchingForDevices();

  /**
   * Function which take care about transmission.
   */
  void transmission();

  /**
   * Function shows ending screen. (maybe unused in future).
   */
  void endingScreen();

  /**
   * Function shows preparing transmission screen.
   */
  void prepareTransmission();

  /**
   * Function show check on screen to adjust camera position.
   */
  void showCheck();

  /**
   * Function process ending dialog response.
   * @param response Ending dialog response.
   */
  void processEndingDialog(int response);

  /**
   * Function test if program should run now.
   * @return true if program should run another time, else otherwise.
   */
  bool isRun() const;
  
  /**
   * Function return information if starting dialog should show.
   * @return true if starting dialog should show, else otherwise.
   */
  bool isShowStartingDialog() const;

  /**
   * Function which sets parameters.
   * @param par Object Parameters with parameters to set.
   */
  void setParameters(Parameters & par);

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
  
  //control variables
  //if program should end
  bool end;
  //if show detecting screen part
  bool show_detect_screen;
  //if show starting dialog
  bool show_starting_dialog;
  //if show device recognition
  bool show_device_recognition;
  
  //time to sleep
  int time;
};

