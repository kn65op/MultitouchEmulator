/** This file contains functions for manipulating images */

#include <opencv2\opencv.hpp>

/**
  * Function that substrat two matrixes in grey scale and return matrix form 0 to 255;
  */
void subtractAbs(cv::Mat & s1, cv::Mat & s2, cv::Mat & dest);

/**
  * Function creates image with stripes. Type of stripes are selected by number parameter
  */
void createStripesImage(cv::Mat & dest, int height, int width, int stripe_size, int number);

/**
  * Function creates image with stripes. Type of stripes are selected by number parameter
  */
void createStripesImage(cv::Mat & dest, cv::Size size, int stripe_size, int number);

/**
  * Function returns screen resolution as cv::Size
  */
cv::Size getScreenResolution();

/**
  * Function that calculates negation of binary image (values 0, 255).
  */
void negation(cv::Mat & im);