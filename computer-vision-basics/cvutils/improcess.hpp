#ifndef __IMPROCESS_HPP__
#define __IMPROCESS_HPP__

#include "opencv2/opencv.hpp"

namespace cvutils {
    //!
    //! \brief Translate an image.
    //! \param src Input image.
    //! \param dst Output image that has the same size and type as src.
    //! \param tx  Value of Tx in the transformation matrix.
    //! \param ty  Value of Ty in the transformation matrix.
    //!
    void translate(cv::InputArray src, cv::OutputArray dst, float tx, float ty);

    //!
    //! \brief Rotate an image.
    //! \param src    Input image.
    //! \param dst    Output image that has the same size and type as src.
    //! \param angle  Rotation angle in degrees. 
    //! Positive value means counter-clockwise rotation. The coordinate origin is the top-left corner.
    //! \param scale  Isotropic scale factor.
    //! \param center Vector (std::vector<float>) contains the (x, y) coordinates of the center of the rotation in the source image.
    //! center[0] is the x-coordinate, center[1] is the y-coordinate.
    //!
    void rotate(cv::InputArray src, cv::OutputArray dst, double angle, double scale = 1.0, cv::InputArray center = cv::noArray());

    //!
    //! \brief Resize an image.
    //! To keep the aspect ratio, specify either the new width or height of the resized image, 
    //! keep the other value as -1.
    //! To shrink an image, it will generally look best with cv::INTER_AREA interpolation, 
    //! whereas to enlarge an image, it will generally look best with cv::INTER_CUBIC (slow) or 
    //! cv::INTER_LINEAR (faster but still looks OK).
    //!
    //! \param src    Input image.
    //! \param dst    Output image that has the same type as src.
    //! \param width  Width of resized image.
    //! \param height Height of the resized image.
    //! \param inter  Interpolation method.
    //!
    void resize(cv::InputArray src, cv::OutputArray dst, int width = -1, int height = -1, int inter = cv::INTER_AREA);
}

#endif  // __IMPROCESS_HPP__
