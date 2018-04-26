#include <cstdio>
#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"

using namespace cv;

int main(void) {
    // Load the image
    Mat image = imread("circles-squares.png");
    if (image.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Original", image);
    waitKey();

    // Convert the input image to grayscale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Find all the external contours
    std::vector<std::vector<Point> > contours;
    findContours(gray.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Loop over the contours
    for (unsigned int i = 0; i < contours.size(); i++) {
        // Approximate the contour
        // Commonly, we define the epsilon as some percentage 
        // (usually between 1-5%) of the original contour perimeter
        double perimeter = arcLength(contours[i], true);
        double epsilon = 0.01 * perimeter;
        std::vector<Point> approx_contour;
        approxPolyDP(contours[i], approx_contour, epsilon, true);

        // If the approximated contour has 4 vertices, then we are examining a rectangle
        if (approx_contour.size() == 4) {
            // Draw the outline and put the text
            drawContours(image, contours, i, COLORNAME_GREEN, 2);
            Rect box = boundingRect(contours[i]);
            putText(image, "Rectangle", Point(box.x, box.y - 10), FONT_HERSHEY_SIMPLEX, 0.5, COLORNAME_GREEN, 2);
        }
    }
    imshow("Output", image);
    waitKey();

    return 0;
}
