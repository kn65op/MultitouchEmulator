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

/**
  * Function to index elements on image. Obects must be black.
  */
void indexImageBlack(cv::Mat & source, cv::Mat & index);

/**
  * Functions, which creates all white image
  */
void createWhiteImage(cv::Mat & image, int x, int y);

/**
  * Function, which creates all white image
  */
void createWhiteImage(cv::Mat & image, cv::Size & size);

/**
  * Function showing window at (0,0) positions without Windows frame (for example window name)
  */
void showImageWithoutFrame(LPCWSTR name, int width, int height);

/**
  * Function showing window specified position without Windows frame (for example window name)
  */
void showImageWithoutFrame(LPCWSTR name, int x, int y, int width, int height);