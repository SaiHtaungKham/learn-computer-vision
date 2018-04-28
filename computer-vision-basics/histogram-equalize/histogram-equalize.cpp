#include <cstdio>
#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{ help   | | Print this message }"
        "{ @input | | Path to the image  }";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Equalize the histogram");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("histogram-equalize beach.png\n");
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

    // Equalize the histogram of image
    Mat equalized;
    equalizeHist(gray, equalized);
    imshow("Equalized", equalized);
    waitKey();
    destroyAllWindows();

    // Compute the histograms of original and equalized images
    std::vector<int> channels = { 0 };
    std::vector<int> hist_size = { 256 };
    std::vector<float> ranges = { 0.0, 256.0 };
    std::vector<Mat> gray_images = { gray };
    std::vector<Mat> eq_images = { equalized };
    Mat gray_hist;
    Mat eq_hist;
    calcHist(gray_images, channels, Mat(), gray_hist, hist_size, ranges);
    calcHist(eq_images, channels, Mat(), eq_hist, hist_size, ranges);
    normalize(gray_hist, gray_hist, 1.0, NORM_L1);
    normalize(eq_hist, eq_hist, 1.0, NORM_L1);

    // Draw the histograms of original and equalized images
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound(static_cast<double>(hist_w) / hist_size[0]);
    Mat gray_hist_image(hist_h, hist_w, CV_8UC3, COLORNAME_WHITE);
    Mat eq_hist_image(hist_h, hist_w, CV_8UC3, COLORNAME_WHITE);
    normalize(gray_hist, gray_hist, 0, eq_hist_image.rows, NORM_MINMAX);
    normalize(eq_hist, eq_hist, 0, eq_hist_image.rows, NORM_MINMAX);
    for (int i = 1; i < hist_size[0]; ++i) {
        line(gray_hist_image, Point(bin_w * (i - 1), hist_h - cvRound(gray_hist.at<float>(i - 1))),
                              Point(bin_w * i, hist_h - cvRound(gray_hist.at<float>(i))),
                              COLORNAME_BLACK, 1, LINE_AA);
        line(eq_hist_image, Point(bin_w * (i - 1), hist_h - cvRound(eq_hist.at<float>(i - 1))),
                            Point(bin_w * i, hist_h - cvRound(eq_hist.at<float>(i))),
                            COLORNAME_BLACK, 1, LINE_AA);
    }

    // Show the histograms of original and equalized images
    imshow("Grayscale Histogram", gray_hist_image);
    imshow("Equalized Histogram", eq_hist_image);
    waitKey();

    return 0;
}
