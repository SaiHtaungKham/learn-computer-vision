#include <cstdio>
#include <cmath>
#include <algorithm>
#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"
#include "cvutils/improcess.hpp"

using namespace cv;

int main(int argc, char** argv) {
    // Define the arguments
    const String keys =
        "{ help   | | Print this message }"
        "{ @input | | Path to the image  }";

    // Construct the command line parser and parse the arguments
    CommandLineParser parser(argc, argv, keys);
    parser.about("Find the document page");

    if (parser.has("help")) {
        parser.printMessage();
        printf("Example:\n");
        printf("contours-approx-scanner receipt.png\n");
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

    // Convert the input image to grayscale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Detect the edges
    Mat edges;
    //Canny(gray, edges, 75, 200);
    cvutils::CannyAuto(gray, edges, 3.0);

    // Debug: show the edge map
    imshow("Edges", edges);

    // Find all the external contours
    std::vector<std::vector<Point> > contours;
    findContours(edges.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Sort the contours from largest to smallest
    std::sort(contours.begin(), contours.end(), cvutils::isLargerContourArea);

    // Loop over the largest contours
    std::vector<std::vector<Point> > approx_contours;
    for (unsigned int i = 0; i < 3; i++) {
        // Approximate the contour
        // Commonly, we define the epsilon as some percentage 
        // (usually between 1-5%) of the original contour perimeter
        double perimeter = arcLength(contours[i], true);
        double epsilon = 0.01 * perimeter;
        std::vector<Point> approx_contour;
        approxPolyDP(contours[i], approx_contour, epsilon, true);
        approx_contours.push_back(approx_contour);

        // Debug: show the difference in number of vertices between the original and approximated contours
        printf("Original: %zu, approx: %zu\n", contours[i].size(), approx_contour.size());

        // If the approximated contour has 4 vertices, then we are examining a rectangle
        if (approx_contour.size() == 4) {
            // Draw the approximated outline of documents
            drawContours(image, approx_contours, -1, COLORNAME_GREEN, 2, LINE_AA);
        }
    }
    imshow("Output", image);
    waitKey();

    return 0;
}
