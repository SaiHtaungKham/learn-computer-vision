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
    parser.about("Apply the connected-components labeling");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("connected-components-labeling license-plate.png\n");
        return 0;
    }

    String input_path = parser.get<String>(0);

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    // Load the image
    Mat image = imread(input_path);
    if (image.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Original", image);
    waitKey();

    // Convert the image to HSV color space
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    // Split the HSV image into 3 channels
    std::vector<Mat> splitted;
    split(hsv, splitted);
    Mat V = splitted[2];

    // Apply the adaptive thresholding
    Mat thresh;
    adaptiveThreshold(V, thresh, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 29, 15.0);
    imshow("Threshold", thresh);
    waitKey();

    // Apply the connected-component analysis
    Mat centroids;
    Mat stats;
    Mat labels;
    int nccomps = connectedComponentsWithStats(thresh, labels, stats, centroids, 8, CV_32S, CCL_WU);
    printf("[INFO] Found %d blobs\n", nccomps);

    Mat mask = Mat::zeros(thresh.size(), CV_8UC1);
    for (int i = 0; i < mask.rows; ++i) {
        for (int j = 0; j < mask.cols; ++j) {
            int label = labels.at<int>(i, j);
            if (label == 0) {
                // This is the background label, skip it
                continue;
            }
            // If the number of pixels in the component is sufficiently large, 
            // add it to our mask
            int ccarea = stats.at<int>(label, CC_STAT_AREA);
            if (ccarea > 300 && ccarea < 1500) {
                mask.at<uchar>(i, j) = 255;
            }
        }
    }
    imshow("Large blobs", mask);
    waitKey();

    // Visually demonstrate how the connected components analysis works
    Mat label_mask = Mat::zeros(thresh.size(), CV_8UC1);
    for (int j = 0; j < mask.cols; ++j) {
        for (int i = 0; i < mask.rows; ++i) {
            int label = labels.at<int>(i, j);
            if (label == 0) {
                printf("[INFO] label: 0 (background)\n");
                continue;
            }
            
            int ccarea = stats.at<int>(label, CC_STAT_AREA);
            if (ccarea > 300 && ccarea < 1500) {
                label_mask.at<uchar>(i, j) = 255;
                printf("[INFO] label: %d, area: %d (foreground) - O\n", label, ccarea);
            }
            else {
                label_mask.at<uchar>(i, j) = 100;
                printf("[INFO] label: %d, area: %d (foreground) - X\n", label, ccarea);
            }
            imshow("Label", label_mask);
            waitKey(1);
        }
    }

    return 0;
}
