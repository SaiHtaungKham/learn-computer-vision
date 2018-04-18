#include <cstdio>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{help   | | Print this message}"
        "{@input | | Path to the image}";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Perform morphological operations");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("morphology-hats car-number-plate\n");
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

    // Convert the input image to grayscale
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    return 0;
}
