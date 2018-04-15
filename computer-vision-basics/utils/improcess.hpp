#ifndef __IMPROCESS_HPP__
#define __IMPROCESS_HPP__

#include "opencv2/opencv.hpp"

//!
//! \brief Translate the image
//! \param src Input image
//! \param dst Output image that has the same size and type as src
//! \param tx  Value of Tx in the transformation matrix
//! \param ty  Value of Ty in the transformation matrix
//!
void translate(const cv::Mat& src, cv::Mat& dst, float tx, float ty);

//!
//! \brief Rotate the image
//! \param src    Input image
//! \param dst    Output image that has the same size and type as src
//! \param angle  Rotation angle in degrees. 
//! Positive value means counter-clockwise rotation. The coordinate origin is the top-left corner.
//! \param scale  Isotropic scale factor
//! \param center Vector (std::vector<float>) contains the (x,y) coordinates of the center of the rotation in the source image.
//! center[0] is the x-coordinate, center[1] is the y-coordinate
//!
void rotate(const cv::Mat& src, cv::Mat& dst, double angle, double scale = 1.0, cv::InputArray center = cv::noArray());

#endif  // __IMPROCESS_HPP__
