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
    parser.about("Compute the color histogram in 2D");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("histogram-color2d beach.png\n");
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

    // Split the image into 3 channels: B, G, R
    std::vector<Mat> splitted;
    split(image, splitted);

    // Compute the histograms: G-B, G-R, B-G
    std::vector<int> channels = { 0, 1 };
    std::vector<int> hist_size = { 32, 32 };
    std::vector<float> ranges = { 0.0, 256.0, 0.0, 256.0 };
    std::vector<Mat> images;
    std::array<Mat, 3> hist;
    for (int i = 0; i < 3; ++i) {
        images.clear();
        if (i == 0) { // G-B
            images = { splitted[1] , splitted[0] };
        }
        else if (i == 1) { // G-R
            images = { splitted[1] , splitted[2] };
        }
        else if (i == 2) { // B-R
            images = { splitted[0] , splitted[2] };   
        }
        calcHist(images, channels, Mat(), hist[i], hist_size, ranges);
    }

    // Draw the 2D histograms
    int scale = 10;
    Mat zeros = Mat::zeros(32 * scale, 32 * scale, CV_8UC3);
    std::array<Mat, 3> hist_img = { zeros, zeros.clone(), zeros.clone() };
    for (int i = 0; i < 3; ++i) {
        double max_val = 0.0;
        minMaxLoc(hist[i], NULL, &max_val);
        // On (row m, column n), draw the intensity rectangle with the top-left coordinates (x=n, y=m) 
        for (int m = 0; m < 32; ++m) {
            for (int n = 0; n < 32; ++n) {
                float bin_val = hist[i].at<float>(m, n);
                double intensity = 255.0 * bin_val / max_val;
                rectangle(hist_img[i], Point(n*scale, m*scale),
                                       Point((n+1)*scale-1, (m+1)*scale-1),
                                       Scalar::all(intensity), FILLED);
            }
        }
    }

    // Show the 2D histograms
    imshow("G-B Histogram", hist_img[0]);
    imshow("G-R Histogram", hist_img[1]);
    imshow("B-R Histogram", hist_img[2]);
    waitKey();
    return 0;
}
