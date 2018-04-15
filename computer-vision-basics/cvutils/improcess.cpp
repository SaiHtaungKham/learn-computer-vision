#include "improcess.hpp"

namespace cvutils {
    using namespace cv;

    void translate(InputArray src, OutputArray dst, float tx, float ty) {
        CV_Assert(src.getMat().total() != 0 &&
            (src.getMat().channels() == 1 || src.getMat().channels() == 3));
        CV_Assert(dst.isMat());

        float data[2][3] = { {1.0, 0.0, tx},
                            {0.0, 1.0, ty} };
        Mat M(2, 3, CV_32F, &data);
        warpAffine(src, dst, M, src.size());
    }


    void rotate(InputArray src, OutputArray dst, double angle, double scale, InputArray center) {
        CV_Assert(src.getMat().total() != 0 &&
            (src.getMat().channels() == 1 || src.getMat().channels() == 3));
        CV_Assert(dst.isMat());

        Point2f center_point;
        if (center.empty()) {
            // If the center is empty, set the center point to the center of the input image
            center_point.x = static_cast<float>(src.getMat().cols * 0.5);
            center_point.y = static_cast<float>(src.getMat().rows * 0.5);
        }
        else {
            CV_Assert(center.isVector());
            CV_Assert(center.total() == 2);
            center_point.x = center.getMat().at<float>(0);
            center_point.y = center.getMat().at<float>(1);
        }
        Mat M = getRotationMatrix2D(center_point, angle, scale);
        warpAffine(src, dst, M, src.size());
    }
}
