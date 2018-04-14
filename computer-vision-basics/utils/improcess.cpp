#include "improcess.hpp"

using namespace cv;

void translate(const Mat& src, Mat& dst, float tx, float ty) {
    float data[2][3] = {{1.0, 0.0, tx},
                        {0.0, 1.0, ty}};
    Mat M(2, 3, CV_32F, &data);
    warpAffine(src, dst, M, src.size());
}
