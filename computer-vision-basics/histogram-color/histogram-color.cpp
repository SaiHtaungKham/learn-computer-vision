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
    parser.about("Compute the color histogram");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("histogram-color beach.png\n");
        return 0;
    }

    String input_path = parser.get<String>(0);

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Load the image and convert it to grayscale
    Mat image = imread(input_path, IMREAD_COLOR);
    if (image.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Image", image);
    waitKey();

    // Compute the histogram:
    // Split the BGR image into 3 separated channels,
    // then compute the histogram of each channel
    std::vector<Mat> splitted;
    split(image, splitted);
    std::vector<int> channels = { 0 };
    std::vector<int> hist_size = { 256 };
    std::vector<float> ranges = { 0.0, 256.0 };
    std::vector<Mat> hist(image.channels());
    for (int i = 0; i < image.channels(); i++) {
        std::vector<Mat> img = { splitted[i] };
        calcHist(img, channels, Mat(), hist[i], hist_size, ranges);
        // Normalize the histogram
        normalize(hist[i], hist[i], 1.0, NORM_L1);
    }
    
    // Draw a "flattened" RGB historgram
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound(static_cast<double>(hist_w) / hist_size[0]);
    Mat hist_image(hist_h, hist_w, CV_8UC3, COLORNAME_WHITE);
    Mat hist_norm;
    Scalar colors[] = { COLORNAME_BLUE, COLORNAME_GREEN, COLORNAME_RED };
    for (int i = 0; i < image.channels(); i++) {
        normalize(hist[i], hist_norm, 0, hist_image.rows, NORM_MINMAX);
        for (int j = 1; j < hist_size[0]; j++) {
            line(hist_image, Point(bin_w * (j - 1), hist_h - cvRound(hist_norm.at<float>(j - 1))),
                             Point(bin_w * j, hist_h - cvRound(hist_norm.at<float>(j))),
                             colors[i], 1, LINE_AA);
        }
    }
    imshow("Color Histogram", hist_image);
    waitKey();

    return 0;
}
