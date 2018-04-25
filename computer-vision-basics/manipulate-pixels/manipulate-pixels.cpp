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
    parser.about("Manipulating pixels");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("manipulate-pixel messi5.jpg\n");
        return 0;
    }
    
    String input_path = parser.get<String>(0);

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Load the image, get its dimensions, and display the image
    Mat img = imread(input_path);
    if (img.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    unsigned int w = img.cols;
    unsigned int h = img.rows;
    imshow("Original", img);
    
    // Get the color intensity value of the pixel at (0, 0)
    Vec3b color_val;
    color_val = img.at<Vec3b>(0, 0);
    printf("Pixel at (0, 0) - Red: %d, Green: %d, Blue: %d\n", color_val[0], color_val[1], color_val[2]);

    // Change the value of the pixel at (0, 0) and make it green
    img.at<Vec3b>(0, 0) = Vec3b(0, 255, 0);
    color_val = img.at<Vec3b>(0, 0);
    printf("Pixel at (0, 0) - Red: %d, Green: %d, Blue: %d\n", color_val[0], color_val[1], color_val[2]);

    // Compute the center of the image
    unsigned int cI = h / 2;  // center of row axis
    unsigned int cJ = w / 2;  // center of column axis
    
    // Access the top-left. top-right, bottom-left, bottom-right corners of the image
    Mat tl = img(Range(0, cI), Range(0, cJ));
    Mat tr = img(Range(0, cI), Range(cJ, w));
    Mat bl = img(Range(cI, h), Range(0, cJ));
    Mat br = img(Range(cI, h), Range(cJ, w));
    imshow("Top-Left Corner", tl);
    imshow("Top-Right Corner", tr);
    imshow("Bottom-left Corner", bl);
    imshow("Bottom-right Corner", br);

    // Make the top-left corner of the original image blue
    for (unsigned i = 0; i < cI; i++) {
        for (unsigned j = 0; j < cJ; j++) {
            img.at<Vec3b>(i, j) = Vec3b(255, 0, 0);
        }
    }
    imshow("Updated", img);

    waitKey(0);
    return 0;
}
