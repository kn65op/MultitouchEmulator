#pragma once

#include <opencv2\opencv.hpp>

#include <random>

#include "Parameters.h"

class Devices;

/**
 * Class responsible for transform image to screen points.
 */
class Homography
{
public:
  /**
   * Constructor, which set all neccesary parameters.
   */
  Homography(void);
  /**
   * Default destructor.
   */
  ~Homography(void);
  
  /**
   * Function creates transformation to convert image points to real_points.
   * @param image_points Array of points corresponding to corners of screen on image.
   * @param real_points Array of points corresponding to cornsers of screen in generated image.
   */
  void runHomography(cv::Mat image_points, cv::Mat real_points);
  /**
   * Function creates transformation to convert image points to real_points. This version assume that all generated image show the screen. Before use this function you should set size of images.
   * @param image_points Array of points corresponding to corners of screen on image.
   */
  void runHomography(cv::Mat image_points);

  /**
   * Function converts points form image to real point.
   * @param x Value of point in OX.
   * @param y Value of point in OY.
   * @return Real point as cv::Point.
   */
  cv::Point getRealPoint(int x, int y);

  /**
   * Function use to manualy slecting points on image.
   * @param event Event.
   * @param x Value of point in OX.
   * @param y Value of point in OY.
   * @param flags Flags.
   * @param param Pointer to Homography object.
   */
  friend void getPointsFromImage(int event, int x, int y, int flags, void* param);

  /**
   * Function setting generated image size.
   * @param x Width of generated image size.
   * @param y Height of generated image size.
   */
  void setGeneratedImageSize(int x, int y);
  /**
   * Function setting generated image size.
   * @param size Size of generated image size.
   */
  void setGeneratedImageSize(cv::Size size);

  /**
   * Function return information if all four points while manually selecting screen corners is setted.
   * @return true if four points is selected, false otherwise.
   */
  bool isPointsSet();

  /**
   * Function that transforms image to genrated image.
   * @param image Image from camera.
   * @return Generated image as cv::Mat.
   */
  cv::Mat & processImage(cv::Mat & image);

  /**
   * Function setting image size.
   * @param x Width of image size.
   * @param y Height of image size.
   */
  void setImageSize(int x, int y);
  /**
   * Function setting image size.
   * @param size Size of image size.
   */
  void setImageSize(cv::Size size);

  /**
   * Set ROI on image from camera (unused at this point).
   * @param frame Image from camera.
   */
  void setROI(cv::Mat & frame) const;

  /**
    * Function returns image with GUI for detecting devices.
    */
  cv::Mat & getGUIDetectDevice(Devices & devs);
  /**
    * Function returns image with GUI with still screen text to wait when camera can see green screen.
    */
  cv::Mat & getGUIStillScreen();
  /**
    * Function returns image with GUI for detecting screen.
    */
  cv::Mat & getGUIDetectScreen();
  /**
    * Function returns image with GUI for transmission.
    */
  cv::Mat & getGUITransmission(Devices & devs);
  /**
    * Function returns image with GUI for entering camera position.
    */
  cv::Mat & getGUICameraPosition();
  /**
    * Function returns image with GUI for entering camera position.
    */
  cv::Mat & getGUIBlackScreen();
  /**
    * Function returns image with GUI for end.
    */
  cv::Mat & getGUIEnd();

  /**
   * Function returns information if transmission is over.
   * @return true if transmission is over, false otherwise.
   */
  bool isEnd() const;

  /**
   * Function returns factor using to shift device.
   * @return Factor in X dimension as double.
   */
  double getShiftX() const;
  /**
   * Function returns factor using to shift device.
   * @return Factor in Y dimension as double.
   */
  double getShiftY() const;

  /**
   * Function returns camera position.
   * @return camera position in X dimension as double.
   */
  double getCameraX() const;
  /**
   * Function returns camera position.
   * @return camera position in Y dimension as double.
   */
  double getCameraY() const;

  /**
   * Function prepares screen to recognize devices.
   */
  void prepareDeviceRecognition();
  /**
   * Function prepares screen to transmission.
   */
  void prepareTransmission(Devices & devs);
  /**
   * Function which set parameters of scene (camera height etc.)
   * @param par Object Parameters with parameters to set.
   */
  void setParameters(Parameters & par);

private:
  /**
   * Function creates transformation to convert image points to real_points.
   */
  void runHomography();

  /**
   * Function creates LUT table in case of performance.
   */
  void makeLUT();

  //Homography matrix
  cv::Mat H;

  //LUT table
  cv::Mat LUT;
  
  //GUI image
  cv::Mat GUI;

  //Points used to make homography
  cv::Mat image_points;
  cv::Mat real_points;

  //size of generate image
  int generated_x;
  int generated_y;
  int level;

  //used in manually selecting screen 
  bool points_setted;

  //generated image
  cv::Mat generated;

  // size of image from camera
  int image_x, image_y;

  //minimum and maksimum coordinates of screen on picture from camera, use to improve performacne
  int min_x, max_x, min_y, max_y;
  
  /**
   * Function check and set minimum and maksimum coordinates of screen on picture from camera.
   * @param x Actual x coordinate.
   * @param y Actual y coordinate/
   */
  void setMinMax(int x, int y);

  /**
   * Function clear GUI (only above line) with actual color.
   */
  void clearGUI(cv::Scalar & scalar);
  /**
   * Function resets homography
   */
  void reset();

  //colors of GUI
  cv::Scalar colorGUI;
  cv::Scalar color_detect_screen;
  cv::Scalar color_line;
  cv::Scalar color_black;
  cv::Scalar color_white;
  cv::Scalar color_gray;

  /**
   * Function sets actual GUI color.
   * @param scalar Actual GUI color to be setted.
   */
  void setGUIColor(cv::Scalar & scalar);

  /**
   * Function does random blink of GUI image.
   */
  void randomBlink();

  //height
  double ratio_x, ratio_y;

  //scale
  double scale, h, h1;
  //camera position
  double camera_pos_x, camera_pos_y;

  //random for mistakes
  std::random_device rd_mistakes;
  std::mt19937 *gen_mistakes;
  std::uniform_int_distribution<> *dist_mistakes;
  
  //probability of mistakes during transmission
  double mistake_posibility;
  
  //tell about end
  bool end;

  //for transmission
  int transmission_progress;
  int transmission_length;
};
