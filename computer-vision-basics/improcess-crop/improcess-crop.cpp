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
    parser.about("Crop the image");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("improcess-crop messi5.jpg\n");
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

    // Crop the face from the image
    Mat face = img(Range(60, 140), Range(200, 280));
    imshow("Face", face);

    // Crop the body from the image
    Mat body = img(Range(60, 250), Range(200, 280));
    imshow("Body", body);

    waitKey();
    return 0;
}
