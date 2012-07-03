/** This file contains functions for manipulating images */

#include <opencv2\opencv.hpp>
#include <vector>

#include "Devices.h"

/**
 * Function that substract two matrixes in grey scale and return matrix form 0 to 255
 * @param s1 Firs matrix
 * @param s2 Second matrix.
 * @param dest Output matrix.
 */
void subtractAbs(cv::Mat & s1, cv::Mat & s2, cv::Mat & dest);

/**
 * Function creates on of two image with stripes. Type of stripes are selected by number parameter
 * @param dest Output image.
 * @param height Height of output image.
 * @param width Width of output image.
 * @param stripe_size Size of stripes.
 * @param number Number of image (0 or 1), poiting to which image is gerated.
 */
void createStripesImage(cv::Mat & dest, int height, int width, int stripe_size, int number);

/**
 * Function creates on of two image with stripes. Type of stripes are selected by number parameter
 * @param dest Output image.
 * @param size Size of output image.
 * @param stripe_size Size of stripes.
 * @param number Number of image (0 or 1), poiting to which image is gerated.
 */
void createStripesImage(cv::Mat & dest, cv::Size size, int stripe_size, int number);

/**
 * Function returns screen resolution as cv::Size
 * @return cv::Size with screen resolution.
 */
cv::Size getScreenResolution();

/**
 * Function that calculates negation of binary image (values 0, 255).
 * @param im Image of which negation is calculated, also output image.
 */
void negation(cv::Mat & im);

/**
 * Function to index elements on image. Obects is black and background is white.
 * @param source Image to label.
 * @param index Output image with indexed objects.
 * @param devs Devices which stores found devices.
 */
void indexImageBlack(cv::Mat & source, cv::Mat & index, Devices & devs);

/**
 * Functions, which creates all white image.
 * @param image Output image.
 * @param x Width of image.
 * @param y Height of image.
 */
void createWhiteImage(cv::Mat & image, int x, int y);

/**
 * Functions, which creates all white image.
 * @param image Output image.
 * @param size Size of image.
 */void createWhiteImage(cv::Mat & image, cv::Size & size);

/**
 * Function showing window at (0,0) positions without Windows frame (for example window name).
 * @param name Name of window to be shown.
 * @param width Demanded width.
 * @param height Demanded Height
 */
void showImageWithoutFrame(LPCWSTR name, int width, int height);

/**
 * Function showing window specified position without Windows frame (for example window name).
 * @param name Name of winow to be shown.
 * @param x Position of window in x dimension.
 * @param y Position of window in y dimenion.
 * @param width Demanded width.
 * @param height Demanded height.
 */
void showImageWithoutFrame(LPCWSTR name, int x, int y, int width, int height);

/**
 * Function showing window at (0,0) positions without Windows frame (for example window name).
 * @param name Name of window to be shown.
 * @param size Demanded size.
 */
void showImageWithoutFrame(LPCWSTR name, cv::Size size);

/**
 * Function test if images are equal. (It probably doesn't work well, but it is unused).
 * @param one First image.
 * @param two Second image.
 * @return true if images are equal (or very similar).
 */
bool isSameImage(cv::Mat & one, cv::Mat & two);

/**
 * Function creates color pattern image, use only while debugging.
 * @param pat Output array.
 */
void generatePattern(cv::Mat & pat);

/**
 * Funtions calculates distance between two points
 * @param a First point.
 * @param b Second point.
 * @return Calculated distance as double.
 */
double distance(cv::Point2f & a, cv::Point2f & b);

/**
 * Functions add to image black check. It's used to calculate camera position.
 * @param image Output image.
 */
void makeCheck(cv::Mat & image);