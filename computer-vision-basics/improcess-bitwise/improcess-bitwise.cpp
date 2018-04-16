#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Draw a rectangle
    Mat rectangle = Mat::ones(300, 300, CV_8UC1);
    cv::rectangle(rectangle, Point(25, 25), Point(275, 275), Scalar(255.0), FILLED);
    imshow("Rectangle", rectangle);

    // Draw a circle
    Mat circle = Mat::ones(300, 300, CV_8UC1);
    cv::circle(circle, Point(150, 150), 150, Scalar(255.0), FILLED);
    imshow("Circle", circle);
    waitKey();

    // Bitwise AND
    Mat bitwiseAnd;
    bitwise_and(rectangle, circle, bitwiseAnd);
    imshow("AND", bitwiseAnd);
    waitKey();

    // Bitwise OR
    Mat bitwiseOr;
    bitwise_or(rectangle, circle, bitwiseOr);
    imshow("OR", bitwiseOr);
    waitKey();

    // Bitwise XOR
    Mat bitwiseXor;
    bitwise_xor(rectangle, circle, bitwiseXor);
    imshow("XOR", bitwiseXor);
    waitKey();

    // Bitwise NOT
    Mat bitwiseNot;
    bitwise_not(circle, bitwiseNot);
    imshow("NOT", bitwiseNot);
    waitKey();

    return 0;
}
