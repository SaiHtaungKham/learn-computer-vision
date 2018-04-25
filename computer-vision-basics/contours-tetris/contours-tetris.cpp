#include <cstdio>
#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"

using namespace cv;

int main(void) {
    // Load the image
    Mat image = imread("tetris-blocks.png");
    if (image.empty()) {
        printf("ERR: Cannot load the image\n");
        return -1;
    }
    imshow("Original", image);
    waitKey();

    // Convert the input image to grayscale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    
    // Binarize, i.e. threshold, the image
    Mat thresh;
    threshold(gray, thresh, 225.0, 255.0, THRESH_BINARY_INV);
    imshow("Thresh", thresh);
    waitKey();

    // Find the external contours in the thresholded image
    std::vector<std::vector<Point> > contours;
    findContours(thresh.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // For each contour
    Mat output = image.clone();
    std::vector<std::vector<Point> > hulls;
    for (unsigned int i = 0; i < contours.size(); i++) {
        // Compute the aspect ratio = width of the bounding box / height of the bounding box
        Rect box = boundingRect(contours[i]);
        double aspect_ratio = box.width / static_cast<double>(box.height);

        // Compute the extent = area of the contour / area of the bounding box
        double area = contourArea(contours[i]);
        double extent = area / static_cast<double>(box.area());

        // Compute the solidity = area of the contour / area of the convex hull
        std::vector<Point> hull;
        convexHull(contours[i], hull);
        hulls.push_back(hull);
        double hull_area = contourArea(hull);
        double solidity = area / hull_area;

        // Recognize the shape of block
        String shape;
        if (aspect_ratio >= 0.98 && aspect_ratio <= 1.02) {
            shape = "SQUARE";
        }
        else if (aspect_ratio >= 3.0) {
            shape = "RECTANGLE";
        }
        else if (extent < 0.65) {
            shape = "L-BLOCK";
        }
        else if (solidity > 0.80) {
            shape = "Z-BLOCK";
        }

        drawContours(output, contours, i, COLORNAME_PURPLE, 3);
        putText(output, shape, Point(box.x, box.y - 10), FONT_HERSHEY_SIMPLEX, 0.5, COLORNAME_PURPLE, 2);
        printf("Contour #%d -- aspect ratio = %.2f, extent = %.2f, solidity = %.2f, shape = %s\n", i + 1, aspect_ratio, extent, solidity, shape.c_str());
    }
    imshow("Output", output);

    // Visualize the convex hulls
    Mat hull_image = Mat::zeros(gray.size(), CV_8UC1);
    drawContours(hull_image, hulls, -1, Scalar(255.0), FILLED);
    imshow("Convex Hull", hull_image);
    waitKey();

    return 0;
}
