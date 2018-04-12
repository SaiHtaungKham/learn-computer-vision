#include "stdafx.h"
#include <cstdio>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys = 
        "{help   | | Print this message}"
        "{@input | | Path to the image}"
        "{output | | Path to the saved image}";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Loading, displaying, and saving image");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("load-display-save lena.jpg --output=new-lena.bmp\n");
        return 0;
    }
    
    String input_path = parser.get<String>(0);
    String output_path;
    if (parser.has("output")) {
        output_path += parser.get<String>("output");
    }

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Load the image and show some of its basic information
    Mat img = imread(input_path);
    if (img.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    printf("Width: %d pixels\n", img.cols);
    printf("Height: %d pixels\n", img.rows);
    printf("Channels: %d\n", img.channels());

    // Display the image and wait for a key press
    imshow("Image", img);
    waitKey(0);

    // Save the image
    // OpenCV automatically converts filetypes
    if (parser.has("output")) {
        imwrite(output_path, img);
    }

    return 0;
}
