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
    parser.about("Resize the image");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("improcess-resize butterfly.jpg\n");
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

    // Calculate the aspect ratio and dimensions of the resized image so that
    // the image does not look skewed or distorted
    double r = 150.0 / img.cols;                          // The output image has the width of 150 pixels
    Size dsize(150, static_cast<int>(img.rows * r));      // The dimensions of the output image

    // Perform the resizing of the input image
    // To shrink an image, it will generally look best with cv::INTER_AREA interpolation
    Mat resized;
    resize(img, resized, dsize, 0.0, 0.0, INTER_AREA);   
    imshow("Resized (Width)", resized);
    waitKey();

    // Calculate the aspect ratio and dimensions of the resized image so that
    // the image does not look skewed or distorted
    r = 50.0 / img.rows;                                // The output image has the height of 50 pixels
    dsize = Size(static_cast<int>(img.cols * r), 50);   // The dimensions of the output image

    // Perform the resizing of the input image
    // To shrink an image, it will generally look best with cv::INTER_AREA interpolation
    resize(img, resized, dsize, 0.0, 0.0, INTER_AREA);
    imshow("Resized (Height)", resized);
    waitKey();

    // Perform the resizing of the input image
    // using the helper function
    cvutils::resize(img, resized, 100);
    imshow("Resized using helper function", resized);
    waitKey();
    destroyAllWindows();

    // Create the list of interpolation methods
    String method_names[] = {"INTER_NEAREST", "INTER_LINEAR", "INTER_AREA", "INTER_CUBIC", "INTER_LANCZOS4"};
    InterpolationFlags methods[] = {INTER_NEAREST, INTER_LINEAR, INTER_AREA, INTER_CUBIC, INTER_LANCZOS4};

    // Increase the image by 3x
    // Iterate over the interpolation methods
    for (unsigned int i = 0; i < sizeof(methods)/sizeof(methods[0]); i++) {
        cvutils::resize(img, resized, img.cols * 3, -1, methods[i]);
        imshow(method_names[i], resized);
    }
    waitKey();

    return 0;
}
