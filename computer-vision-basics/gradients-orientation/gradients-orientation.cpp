#include <cstdio>
#include <cmath>
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{ help        |     | Print this message   }"
        "{ @input      |     | Path to the image    }"
        "{ lower-angle | 175 | Lower angle boundary }"
        "{ upper-angle | 180 | Upper angle boundary }";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Compute the gradient magnitude and orientation");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("gradients-orientation coins-2.jpg\n");
        return 0;
    }

    String input_path = parser.get<String>(0);
    int lowera = parser.get<int>("lower-angle");
    int uppera = parser.get<int>("upper-angle");

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Load the image
    Mat img = imread(input_path);
    if (img.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Original", img);
    waitKey();

    // Convert the input image to grayscale
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Compute the gradients along the X and Y axis, respectively
    // Apply the Sobel operator
    Mat gX;
    Mat gY;
    Sobel(gray, gX, CV_64F, 1, 0);
    Sobel(gray, gY, CV_64F, 0, 1);

    // Compute the gradient magnitude
    Mat squareX;
    Mat squareY;
    Mat mag;
    cv::pow(gX, 2, squareX);
    cv::pow(gY, 2, squareY);
    cv::sqrt(squareX + squareY, mag);

    // Compute the gradient orientation
    Mat orientation(gX.size(), CV_8U);
    for (int i = 0; i < orientation.rows; i++) {
        for (int j = 0; j < orientation.cols; j++) {
            int angle = (int) (std::atan2(gY.at<double>(i, j), gX.at<double>(i, j)) * (180 / CV_PI)) % 180;
            orientation.at<uchar>(i, j) = (uchar) angle;
        }
    }

    // For visualization:
    // Find all pixels which are within the upper and lower angle boundaries
    Mat mask = Mat::zeros(orientation.size(), CV_8U);
    for (int i = 0; i < mask.rows; i++) {
        for (int j = 0; j < mask.cols; j++) {
            if (orientation.at<uchar>(i, j) >= lowera &&
                orientation.at<uchar>(i, j) <= uppera) {
                mask.at<uchar>(i, j) = 255;
            }
        }
    }

    // Show the output image
    imshow("Mask", mask);
    waitKey();

    return 0;
}
