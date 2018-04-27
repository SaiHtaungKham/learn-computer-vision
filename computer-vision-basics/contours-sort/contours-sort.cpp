#include <cstdio>
#include <algorithm>
#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"
#include "cvutils/improcess.hpp"

using namespace cv;

static void drawContourNum(InputOutputArray image, InputArray contour, int idx);

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{ help    | | Print this message             }"
        "{ @input  | | Path to the image              }"
        "{ @method | | Sorting method: lr, rl, tb, bt }"; // left-right, right-left, top-bottom, bottom-top

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Sort the contours");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("contours-sort lego-blocks-2.png lr\n");
        return 0;
    }

    String input_path = parser.get<String>(0);
    String method = parser.get<String>(1);

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

    // Split the input image into 3 channels: B, G, R
    std::vector<Mat> splitted;
    split(image, splitted);

    // Detect the edges:
    // Extract the edges of each channel, then accumulate them
    Mat edges = Mat::zeros(image.size(), CV_8UC1);
    for (unsigned int i = 0; i < splitted.size(); i++) {
        Mat channel_blurred;
        GaussianBlur(splitted[i], channel_blurred, Size(7, 7), 0.0);
        Mat channel_edges;
        cvutils::CannyAuto(channel_blurred, channel_edges);
        bitwise_or(channel_edges, edges, edges);
    }

    // Debug: show the edge map
    imshow("Edges", edges);

    // Find all the external contours
    std::vector<std::vector<Point> > contours;
    findContours(edges.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Draw the contour number
    Mat unsorted = image.clone();
    for (unsigned int i = 0; i < contours.size(); i++) {
        drawContourNum(unsorted, contours[i], i);
    }
    imshow("Unsorted", unsorted);
    waitKey();

    // Sort the contours
    if (method == "lr" || "rl") {
        std::sort(contours.begin(), contours.end(), cvutils::isLeftToRightContour);
    }
    else if (method == "tb" || method == "bt") {
        std::sort(contours.begin(), contours.end(), cvutils::isTopToBottomContour);
    }

    if (method == "rl" || method == "bt") {
        std::reverse(contours.begin(), contours.end());
    }

    // Draw the contour number (sorted contours)
    Mat sorted = image.clone();
    for (unsigned int i = 0; i < contours.size(); i++) {
        drawContourNum(sorted, contours[i], i);
    }
    imshow("Sorted", sorted);
    waitKey();

    return 0;
}


static void drawContourNum(InputOutputArray image, InputArray contour, int idx) {
    // Compute the center of the contour area
    Moments M = moments(contour);
    int cX = static_cast<int>(M.m10 / M.m00);
    int cY = static_cast<int>(M.m01 / M.m00);

    // Draw the contour number
    putText(image, format("#%d", idx + 1), Point(cX - 20, cY), FONT_HERSHEY_SIMPLEX, 1.0, COLORNAME_WHITE, 2);
}
