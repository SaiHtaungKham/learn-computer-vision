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
    parser.about("Smooth/blur an image");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("smooth-blur butterfly.jpg\n");
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

    // Define some kernel sizes
    Size kernelSizes[] = { Size(3, 3), Size(9, 9), Size(15, 15) };

    // Perform several averagings
    Mat blurred;
    for (unsigned int i = 0; i < 3; i++) {
        blur(img, blurred, kernelSizes[i]);
        imshow(format("Average (%d, %d)", kernelSizes[i].width, kernelSizes[i].height), blurred);
        waitKey();
    }

    // Close windows
    destroyAllWindows();
    imshow("Original", img);

    // Perform several Gaussian blurrings
    for (unsigned int i = 0; i < 3; i++) {
        GaussianBlur(img, blurred, kernelSizes[i], 0.0);
        imshow(format("Gaussian (%d, %d)", kernelSizes[i].width, kernelSizes[i].height), blurred);
        waitKey();
    }

    // Close windows
    destroyAllWindows();
    imshow("Original", img);

    // Define some kernel sizes
    int kSizes[] = { 3, 9, 15 };

    // Perform several median blurrings
    for (unsigned int i = 0; i < 3; i++) {
        medianBlur(img, blurred, kSizes[i]);
        imshow(format("Median %d", kSizes[i]), blurred);
        waitKey();
    }

    // Close windows
    destroyAllWindows();
    imshow("Original", img);

    // Define some parameters
    int diameter = 11;
    double sigmaColors[] = { 21.0, 41.0, 61.0 };
    double sigmaSpaces[] = { 7.0, 21.0, 39.0 };

    // Perform several bilateral blurrings
    for (unsigned int i = 0; i < 3; i++) {
        bilateralFilter(img, blurred, diameter, sigmaColors[i], sigmaSpaces[i]);
        imshow(format("Bilateral d=%d, sc=%.0f, ss=%.0f", diameter, sigmaColors[i], sigmaSpaces[i]), blurred);
        waitKey();
    }

    return 0;
}
