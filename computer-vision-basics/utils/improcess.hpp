#ifndef __IMPROCESS_HPP__
#define __IMPROCESS_HPP__

#include "opencv2/opencv.hpp"

//!
//! \brief Translate the image
//! \param src input image
//! \param dst output image that has the same size and type as src
//! \param tx value of Tx in the transformation matrix
//! \param ty value of Ty in the transformation matrix
//!
void translate(const cv::Mat& src, cv::Mat& dst, float tx, float ty);

#endif  // __IMPROCESS_HPP__
