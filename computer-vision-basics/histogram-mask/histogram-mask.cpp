#include <cstdio>
#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"

using namespace cv;

static void drawHistogram(InputArray image, InputOutputArray hist_image, InputArray mask = Mat());

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{ help   | | Print this message }"
        "{ @input | | Path to the image  }";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Compute the histogram with masking");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("histogram-mask beach.png\n");
        return 0;
    }

    String input_path = parser.get<String>(0);

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Load the image
    Mat image = imread(input_path, IMREAD_COLOR);
    if (image.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Image", image);
    waitKey();

    // Draw the histograms
    // No mask
    Mat hist_image_nomask = Mat(400, 512, CV_8UC3, COLORNAME_WHITE);
    drawHistogram(image, hist_image_nomask, Mat());

    // Create the mask
    Mat mask = Mat::zeros(image.size(), CV_8UC1);
    rectangle(mask, Rect(Point(60, 290), Point(210, 390)), Scalar(255.0), FILLED);
    imshow("Mask", mask);
    
    // Show the masked image
    Mat masked;
    bitwise_and(image, image, masked, mask);
    imshow("Applying the Mask", masked);

    // Draw the histograms (masked)
    Mat hist_image_masked = Mat(400, 512, CV_8UC3, COLORNAME_WHITE);
    drawHistogram(image, hist_image_masked, mask);

    // Show the historgrams
    imshow("Histogram of Original Image", hist_image_nomask);
    imshow("Histogram of Masked Image", hist_image_masked);
    waitKey();

    return 0;
}


void drawHistogram(InputArray image, InputOutputArray hist_image, InputArray mask) {
    // Compute the histogram:
    // Split the BGR image into 3 separated channels,
    // then compute the histogram of each channel
    std::vector<Mat> splitted;
    split(image.getMat(), splitted);
    std::vector<int> channels = { 0 };
    std::vector<int> hist_size = { 256 };
    std::vector<float> ranges = { 0.0, 256.0 };
    std::vector<Mat> hist(image.getMat().channels());
    for (int i = 0; i < image.getMat().channels(); i++) {
        std::vector<Mat> img = { splitted[i] };
        calcHist(img, channels, mask, hist[i], hist_size, ranges);
        // Normalize the histogram
        normalize(hist[i], hist[i], 1.0, NORM_L1);
    }

    // Draw a "flattened" RGB historgram
    int bin_w = cvRound(static_cast<double>(hist_image.getMat().cols) / hist_size[0]);
    Scalar colors[] = { COLORNAME_BLUE, COLORNAME_GREEN, COLORNAME_RED };
    for (int i = 0; i < image.getMat().channels(); i++) {
        normalize(hist[i], hist[i], 0, hist_image.getMat().rows, NORM_MINMAX);
        for (int j = 1; j < hist_size[0]; j++) {
            line(hist_image, Point(bin_w * (j - 1), hist_image.getMat().rows - cvRound(hist[i].at<float>(j - 1))),
                             Point(bin_w * j, hist_image.getMat().rows - cvRound(hist[i].at<float>(j))),
                             colors[i], 1, LINE_AA);
        }
    }
}
