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
    parser.about("Perform morphological operations");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("morphology-opening computervision-blobs.png\n");
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

    // Define some kernel sizes
    Size kernelSizes[] = { Size(3, 3), Size(5, 5), Size(7, 7) };

    // Apply serveral openings
    for (unsigned int i = 0; i < 3; i++) {
        Mat kernel = getStructuringElement(MORPH_RECT, kernelSizes[i]);
        Mat opening;
        morphologyEx(gray, opening, MORPH_OPEN, kernel);
        imshow(format("Opening: (%d, %d)", kernelSizes[i].width, kernelSizes[i].height), opening);
        waitKey();
    }

    return 0;
}
