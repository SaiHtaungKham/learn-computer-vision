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
    parser.about("Split and merge the channels of image");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("improcess-split-merge butterfly.jpg\n");
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

    // Grab each channel
    std::vector<Mat> splitted;
    split(img, splitted);

    // Display each channel individually
    // OpenCV stores an image as BGR order.
    imshow("B", splitted[0]);
    imshow("G", splitted[1]);
    imshow("R", splitted[2]);
    waitKey();

    // Merge the channels
    Mat merged;
    merge(splitted, merged);
    imshow("Merged", merged);
    waitKey();
    destroyAllWindows();

    // For visualization, display each channel in color
    Mat zeros = Mat::zeros(img.size(), CV_8UC1);
    std::vector<Mat> blues = { splitted[0], zeros, zeros };
    std::vector<Mat> greens = { zeros, splitted[1], zeros };
    std::vector<Mat> reds = { zeros, zeros, splitted[2] };
    Mat blue, green, red;
    merge(blues, blue);
    merge(greens, green);
    merge(reds, red);
    imshow("Blue", blue);
    imshow("Greens", green);
    imshow("Red", red);
    waitKey();

    return 0;
}
