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
    parser.about("Perform simple thresholding");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("threshold-simple coins.jpg\n");
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

    // Blur the grayscale image
    Mat blurred;
    GaussianBlur(gray, blurred, Size(7, 7), 0.0);

    // Apply the simple thresholding
    // We seek the segmented coins to appear as white on the black background
    // In the original image, coins are put on the brighter background, hence we use the inversed thresholding 
    double T = 125.0;   // Manually define the threshold value
    Mat threshInv;
    threshold(gray, threshInv, T, 255.0, THRESH_BINARY_INV);
    imshow("Threshold Binary Inversed", threshInv);
    waitKey();

    // Visualize the masked region in the original image by applying the bitwise AND operation
    // Noted that there are small holes inside the thresholed coins
    // We can filled these holes using morphological operations or countour methods
    Mat output;
    bitwise_and(img, img, output, threshInv);
    imshow("Output", output);
    waitKey();

    return 0;
}
