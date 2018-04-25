#include <cstdio>
#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"

using namespace cv;

int main(void) {
    // Load the image
    Mat image = imread("tictactoe.png");
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

    Mat output = image.clone();
    for (unsigned int i = 0; i < contours.size(); i++) {
        // Compute the solidity = area of the contour / area of the convex hull
        double area = contourArea(contours[i]);
        std::vector<Point> hull;
        convexHull(contours[i], hull);
        double hull_area = contourArea(hull);
        double solidity = area / hull_area;

        // Recognize X's and O's
        String character = "?";
        if (solidity > 0.9) {
            character = "O";
        }
        else if (solidity > 0.5) {
            character = "X";
        }

        // Visualize
        if (character != "?") {
            Rect box = boundingRect(contours[i]);
            drawContours(output, contours, i, COLORNAME_PURPLE, 3);
            putText(output, character, Point(box.x, box.y - 10), FONT_HERSHEY_SIMPLEX, 1.25, COLORNAME_PURPLE, 4);
        }

        printf("%s (Contour #%d) -- solidity = %.2f\n", character.c_str(), i + 1, solidity);
    }
    imshow("Output", output);
    waitKey();

    return 0;
}
