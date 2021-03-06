#include "improcess.hpp"

using namespace cv;

void cvutils::translate(InputArray src, OutputArray dst, float tx, float ty) {
    CV_Assert(src.getMat().total() != 0 &&
                (src.getMat().channels() == 1 || src.getMat().channels() == 3));
    CV_Assert(dst.isMat());

    float data[2][3] = { {1.0, 0.0, tx},
                            {0.0, 1.0, ty} };
    Mat M(2, 3, CV_32F, &data);
    warpAffine(src, dst, M, src.size());
}


void cvutils::rotate(InputArray src, OutputArray dst, double angle, double scale, InputArray center) {
    CV_Assert(src.getMat().total() != 0 &&
                (src.getMat().channels() == 1 || src.getMat().channels() == 3));
    CV_Assert(dst.isMat());
    CV_Assert(center.empty() || 
                (center.isVector() && center.total() == 2));

    Point2f center_point;
    if (center.empty()) {
        // If the center is empty, set the center point to the center of the input image
        center_point.x = static_cast<float>(src.getMat().cols * 0.5);
        center_point.y = static_cast<float>(src.getMat().rows * 0.5);
    }
    else {
        center_point.x = center.getMat().at<float>(0);
        center_point.y = center.getMat().at<float>(1);
    }
    Mat M = getRotationMatrix2D(center_point, angle, scale);
    warpAffine(src, dst, M, src.size());
}


void cvutils::resize(cv::InputArray src, cv::OutputArray dst, int width, int height, int inter) {
    CV_Assert(src.getMat().total() != 0 &&
                (src.getMat().channels() == 1 || src.getMat().channels() == 3));
    CV_Assert(dst.isMat());
    CV_Assert((width == -1) && (height == -1) ||
                (width > 0 && height == -1) ||
                (width == -1 && height > 0) ||
                (width > 0 && height > 0));
    CV_Assert(inter == INTER_NEAREST ||
                inter == INTER_LINEAR ||
                inter == INTER_CUBIC ||
                inter == INTER_AREA ||
                inter == INTER_LANCZOS4);

    double r;
    Size dsize;
    if (width == -1 && height == -1) {
        src.copyTo(dst);
    }
    else if (height == -1) {
        r = static_cast<double>(width) / src.getMat().cols;
        dsize = Size(width, static_cast<int>(src.getMat().rows * r));
        resize(src, dst, dsize, 0.0, 0.0, inter);
    }
    else if (width == -1) {
        r = static_cast<double>(height) / src.getMat().rows;
        dsize = Size(static_cast<int>(src.getMat().cols * r), height);
        resize(src, dst, dsize, 0.0, 0.0, inter);
    }
    else {  // Resize the input image but don't assure to keep the aspect ratio
        dsize = Size(width, height);
        resize(src, dst, dsize, 0.0, 0.0, inter);
    }
}


int cvutils::median(InputArray image) {
    CV_Assert(image.getMat().total() != 0 && 
                image.getMat().channels() == 1 &&
                image.getMat().type() == CV_8U);

    int m = (image.getMat().rows * image.getMat().cols) / 2;
    int bin = 0;
    int med = -1;

    int histSize = 256;
    float range[] = { 0.0, 256.0 };
    const float* histRange = { range };
    Mat hist;
    calcHist(&image.getMat(), 1, 0, Mat(), hist, 1, &histSize, &histRange);
    for (int i = 0; i < histSize && med < 0; ++i) {
        bin += cvRound(hist.at<float>(i));
        if (bin > m && med < 0) {
            med = i;
        }
    }

    return med;
}


void cvutils::CannyAuto(InputArray image, OutputArray edges, double sigma) {
    CV_Assert(image.getMat().total() != 0 && 
                image.getMat().channels() == 1 &&
                image.getMat().type() == CV_8U);
    CV_Assert(edges.isMat());

    int v = median(image);
    double lowerT = std::max(0.0, (1.0 - sigma) * v);
    double upperT = std::min(255.0, (1.0 + sigma) * v);
    Canny(image, edges, lowerT, upperT);
}


bool cvutils::isLargerContourArea(InputArray contour1, InputArray contour2) {
    double i = fabs(contourArea(contour1));
    double j = fabs(contourArea(contour2));
    return (i > j);
}


bool cvutils::isLeftToRightContour(InputArray contour1, InputArray contour2) {
    Rect box1 = boundingRect(contour1);
    Rect box2 = boundingRect(contour2);
    return (box1.x < box2.x);
}


bool cvutils::isTopToBottomContour(InputArray contour1, InputArray contour2) {
    Rect box1 = boundingRect(contour1);
    Rect box2 = boundingRect(contour2);
    return (box1.y < box2.y);
}
