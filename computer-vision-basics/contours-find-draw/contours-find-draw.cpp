#include <cstdio>
#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{ help   | | Print this message }"
        "{ @input | | Path to the image  }";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Find and draw contours");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("contours-find-draw shapes.png\n");
        return 0;
    }

    String input_path = parser.get<String>(0);

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Load the image
    Mat image = imread(input_path);
    if (image.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Original", image);
    waitKey();

    // Convert the input image to grayscale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Find all contours in the image
    // The cv::findContours is destructive to the input image, 
    // so we clone the input image before passing it as an argument 
    // if we need to use it again.
    std::vector<std::vector<Point> > contours;
    findContours(gray.clone(), contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    printf("Found %d contours\n", (int)contours.size());

    // Draw all the contours
    Mat clone = image.clone();
    drawContours(clone, contours, -1, COLOR_YELLOW, 2);
    imshow("All Contours", clone);
    waitKey();
    destroyAllWindows();

    // Draw each contour, one by one
    clone = image.clone();
    for (unsigned int i = 0; i < contours.size(); i++) {
        drawContours(clone, contours, i, COLOR_AQUA, 2);
        printf("Draw contours #%d\n", i + 1);
        imshow("Contours", clone);
        waitKey();
    }
    destroyAllWindows();

    // Find only the external contours
    contours.clear();
    findContours(gray.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    printf("Found %d EXTERNAL contours\n", (int)contours.size());
    clone = image.clone();
    drawContours(clone, contours, -1, COLOR_GREEN, 2);
    imshow("External Contours", clone);
    waitKey();
    destroyAllWindows();

    // Construct a map by drawing only the current contour
    imshow("Original", image);
    for (unsigned int i = 0; i < contours.size(); i++) {
        Mat mask = Mat::zeros(image.size(), CV_8UC1);
        Mat masked;
        drawContours(mask, contours, i, Scalar(255.0), FILLED);
        bitwise_and(image, image, masked, mask);
        imshow("Mask", mask);
        imshow("Masked", masked);
        waitKey();
    }

    return 0;
}
