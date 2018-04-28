#include <cstdio>
#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{ help    | | Print this message }"
        "{ @input  | | Path to the image  }";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Compute the grayscale histogram");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("histogram-grayscale beach.png\n");
        return 0;
    }

    String input_path = parser.get<String>(0);

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Load the image and convert it to grayscale
    Mat gray = imread(input_path, IMREAD_GRAYSCALE);
    if (gray.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Grayscale", gray);
    waitKey();

    // Compute the histogram
    std::vector<Mat> images = { gray };
    std::vector<int> channels = { 0 };
    std::vector<int> hist_size = { 256 };
    std::vector<float> ranges = { 0.0, 256.0 }; // the ending range is non-inclusive, then we provide 256 rather than 255
    Mat hist;
    calcHist(images, channels, Mat(), hist, hist_size, ranges);

    // Normalize the histogram
    normalize(hist, hist, 1.0, NORM_L1);

    // Draw the histogram
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound(static_cast<double>(hist_w) / hist_size[0]);
    Mat hist_image(hist_h, hist_w, CV_8UC3, COLORNAME_WHITE);
    Mat hist_norm;
    normalize(hist, hist_norm, 0, hist_image.rows, NORM_MINMAX); // normalize to range [0, 1]
    for (int i = 1; i < hist_size[0]; i++) {
        line(hist_image, Point(bin_w * (i - 1), hist_h - cvRound(hist_norm.at<float>(i - 1))),
                         Point(bin_w * i, hist_h - cvRound(hist_norm.at<float>(i))), 
                         COLORNAME_BLACK, 1, LINE_AA);
    }
    imshow("Grayscale Histogram", hist_image);
    waitKey();

    return 0;
}
