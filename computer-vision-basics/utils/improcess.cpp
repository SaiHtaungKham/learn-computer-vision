#include "improcess.hpp"
#include <iostream>

using namespace cv;

void translate(const Mat& src, Mat& dst, float tx, float ty) {
    float data[2][3] = {{1.0, 0.0, tx},
                        {0.0, 1.0, ty}};
    Mat M(2, 3, CV_32F, &data);
    warpAffine(src, dst, M, src.size());
}

void rotate(const Mat& src, Mat& dst, double angle, double scale, InputArray center) {
    std::vector<Mat> center_vec;
    Point2f center_point;
    if (center.empty()) {
        // If the center is empty, set the center point to the center of the input image
        center_point.x = static_cast<float>(src.cols * 0.5);
        center_point.y = static_cast<float>(src.rows * 0.5);
    }
    else {
        center.getMatVector(center_vec);
        center_point.x = center_vec[0].at<float>(0);
        center_point.y = center_vec[1].at<float>(0);
    }
    Mat M = getRotationMatrix2D(center_point, angle, scale);
    warpAffine(src, dst, M, src.size());
}
