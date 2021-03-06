#include <cstdio>
#include "opencv2/opencv.hpp"
#include "cvutils/improcess.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys = 
        "{ help   | | Print this message }"
        "{ @input | | Path to the image  }";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Translate the image");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("improcess-translate butterfly.jpg\n");
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

    // Translate the image 25 pixels to the right and 50 pixels down
    // OpenCV expects the transformation matrix to be of floating point type
    float data[2][3] = { {1.0, 0.0, 25.0}, 
                         {0.0, 1.0, 50.0} };
    Mat M(2, 3, CV_32F, &data);
    Mat shifted;
    warpAffine(img, shifted, M, img.size());
    imshow("Shifted Down and Right", shifted);
    waitKey();

    // Translate the image 50 pixels to the left and 90 pixels up
    // The pre-existing data area for matrix M has been assigned, so it is only needed to change 
    // the data value to update the transformation matrix.
    data[0][2] = -50.0;
    data[1][2] = -90.0;  
    warpAffine(img, shifted, M, img.size());
    imshow("Shifted Up and Left", shifted);
    waitKey();

    // Translate the image 100 pixels down
    // using the helper function
    cvutils::translate(img, shifted, 0.0, 100.0);
    imshow("Shifted Down", shifted);
    waitKey();

    return 0;
}
