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
        printf("morphology computervision.png\n");
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

    // Apply several erosions
    for (unsigned int i = 0; i < 3; i++) {
        Mat eroded;
        erode(gray, eroded, Mat(), Point(-1, -1), i + 1);
        imshow("Eroded " + format("%d", i + 1) + " times", eroded);
        waitKey();
    }

    // Apply several dilations
    for (unsigned int i = 0; i < 3; i++) {
        Mat dilated;
        dilate(gray, dilated, Mat(), Point(-1, -1), i + 1);
        imshow(format("Dilated %d times", i + 1), dilated);
        waitKey();
    }

    // Close windows
    destroyAllWindows();
    imshow("Original", img);

    // Define some kernel sizes
    Size kernelSizes[] = { Size(3, 3), Size(5, 5), Size(7, 7) };

    // Apply several closings
    for (unsigned int i = 0; i < 3; i++) {
        Mat kernel = getStructuringElement(MORPH_RECT, kernelSizes[i]);
        Mat closing;
        morphologyEx(img, closing, MORPH_CLOSE, kernel);
        imshow(format("Closing: (%d, %d)", kernelSizes[i].width, kernelSizes[i].height), closing);
        waitKey();
    }

    // Close windows
    destroyAllWindows();
    imshow("Original", img);

    // Apply several morphological gradients
    for (unsigned int i = 0; i < 3; i++) {
        Mat kernel = getStructuringElement(MORPH_RECT, kernelSizes[i]);
        Mat gradient;
        morphologyEx(img, gradient, MORPH_GRADIENT, kernel);
        imshow(format("Gradient: (%d, %d)", kernelSizes[i].width, kernelSizes[i].height), gradient);
        waitKey();
    }

    return 0;
}
