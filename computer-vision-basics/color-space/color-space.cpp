#include <cstdio>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{ help   | | Print this message }"
        "{ @input | | Path to the image  }";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Color spaces");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("color-space butterfly.jpg\n");
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
    waitKey();

    // Grab each channel
    std::vector<Mat> splitted;
    split(img, splitted);

    // Display each channel individually
    // OpenCV stores an image as BGR order.
    imshow("B", splitted[0]);
    imshow("G", splitted[1]);
    imshow("R", splitted[2]);
    waitKey();
    destroyAllWindows();

    // Convert the image to the HSV color space
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);
    imshow("HSV", hsv);
    waitKey();

    // Grab each channel and display each channel individually
    splitted.clear();
    split(hsv, splitted);
    imshow("H", splitted[0]);
    imshow("S", splitted[1]);
    imshow("V", splitted[2]);
    waitKey();
    destroyAllWindows();

    // Convert the image to the L*a*b* color space
    Mat lab;
    cvtColor(img, lab, COLOR_BGR2Lab);
    imshow("lab", lab);
    waitKey();

    // Grab each channel and display each channel individually
    splitted.clear();
    split(lab, splitted);
    imshow("L*", splitted[0]);
    imshow("a*", splitted[1]);
    imshow("b*", splitted[2]);
    waitKey();
    destroyAllWindows();

    // Convert the image to the grayscale;
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    imshow("BGR", img);
    imshow("Grayscale", gray);
    waitKey();

    return 0;
}
