#include <cstdio>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{help   | | Print this message}"
        "{@input | | Path to the image}";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Mask the image");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("improcess-mask messi5.jpg\n");
        return 0;
    }

    String input_path = parser.get<String>(0);

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

    // Create a rectangular mask that displays only the body
    Mat mask = Mat::zeros(img.size(), CV_8UC1);
    rectangle(mask, Point(200, 60), Point(280, 250), Scalar(255.0), FILLED);
    imshow("Mask", mask);

    // Apply the mask to crop out the body
    Mat masked;
    bitwise_and(img, img, masked, mask);
    imshow("Mask Applied to Image", masked);
    waitKey();

    // Create a circular mask and apply it
    mask = Mat::zeros(img.size(), CV_8UC1);
    circle(mask, Point(240, 100), 35, Scalar(255.0), FILLED);
    imshow("Mask", mask);
    masked = Mat::zeros(img.size(), CV_8UC3);
    bitwise_and(img, img, masked, mask);
    imshow("Mask Applied to Image", masked);
    waitKey();

    return 0;
}
