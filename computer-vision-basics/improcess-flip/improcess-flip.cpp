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
    parser.about("Flip the image");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("improcess-flip messi5.jpg\n");
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

    // Flip the image around the y-axis (horizontally)
    Mat flipped;
    flip(img, flipped, 1);
    imshow("Flipped Horizontally", flipped);
    waitKey();

    // Flip the image around the x-axis (vertically)
    flip(img, flipped, 0);
    imshow("Flipped Vertically", flipped);
    waitKey();

    // Flip the image around both axes
    flip(img, flipped, -1);
    imshow("Flipped Horizontally & Vertically", flipped);
    waitKey();

    return 0;
}
