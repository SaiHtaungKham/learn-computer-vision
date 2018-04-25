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
    parser.about("Apply the Sobel operator");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("gradients-sobel brick-wall.jpg\n");
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

    // Compute the gradients along the X and Y axis, respectively
    // Apply the Sobel operator
    Mat gX;
    Mat gY;
    Sobel(gray, gX, CV_64F, 1, 0);
    Sobel(gray, gY, CV_64F, 0, 1);

    // For visualization:
    // Convert the gradients to the unsigned 8-bit integer representation
    convertScaleAbs(gX, gX);
    convertScaleAbs(gY, gY);

    // Combine gX and gY to an image
    Mat sobelCombined;
    addWeighted(gX, 0.5, gY, 0.5, 0.0, sobelCombined);

    // Show output images
    imshow("Sobel X", gX);
    imshow("Sobel Y", gY);
    imshow("Sobel Combined", sobelCombined);
    waitKey();

    return 0;
}
