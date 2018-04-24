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
    parser.about("Apply the Canny edge detector");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("canny coins.jpg\n");
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

    // Smooth the image
    Mat blurred;
    GaussianBlur(gray, blurred, Size(5, 5), 0.0);
    imshow("Blurred", blurred);
    waitKey();

    // Apply the Canny edge detector
    Mat edges;

    // Wide threshold
    Canny(blurred, edges, 10.0, 200.0);
    imshow("Wide Edge Map", edges);
    waitKey();

    // Mid threshold
    Canny(blurred, edges, 30.0, 150.0);
    imshow("Mid Edge Map", edges);
    waitKey();

    // Tight threshold
    Canny(blurred, edges, 200.0, 210.0);
    imshow("Tight Edge Map", edges);
    waitKey();

    return 0;
}
