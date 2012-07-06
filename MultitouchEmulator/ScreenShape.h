#pragma once
/**
  * Screen
  */

#include <opencv2\opencv.hpp>
#include <vector>

/**
 * Class coresponds to Screen (cannot be named Screen, beacause of Windows)/.
 */
class ScreenShape
{
public:
  /**
   * Default constructor
   */
  ScreenShape(void);
  /**
   * Default destructor
   */
  ~ScreenShape(void);

  /**
   * Function that detects white screen on binary image and make convexHull on screen points.
   * @param image Binary image
   * @return true if at least one point is from screen (white), else if none point.
   */
  bool detectScreen(cv::Mat & image);
  
  /**
   * Function draws screen shape on image.
   * @param image Image on which screen will be drawned.
   */
  void printScreen(cv::Mat & image);

  /**
   * Funtions returns size of four longest edges.
   * @return std::vector<double> with length of four longest edges.
   */
  std::vector<double> getSize() const;

  /**
   * Function finds screen on binary image and store corners of screen.
   * @param bin Binary image from image of screen with values 0 or 255.
   */
  void findScreenAtBinaryImage(cv::Mat & bin);

  /**
   * Function returns cornsers of screen.
   * @return std::vector<cv::Point> with four points which are screen corners.
   */
  std::vector<cv::Point> getCorners() const;

  /**
   * Function returns all points from screen.
   * @return std::vector<cv::Point> with all points corresponds to screen.
   */
  std::vector<cv::Point> getScreen() const;
  /**
   * Function prepares to another screen recognition
   */
  void prepare();

private:
  //screen points
  std::vector<cv::Point> screen;

  //screen corners points
  std::vector<cv::Point> corners;
};

