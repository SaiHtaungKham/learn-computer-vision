#include "opencv2/opencv.hpp"
#include "cvutils/colors.hpp"

using namespace cv;

int main(void) {
    // Create a canvas as a 300x300 pixel 3-channel image with a black background
    Mat canvas = Mat::zeros(300, 300, CV_8UC3);

    // Draw a green line from the top-left of the canvas to the bottom-right
    line(canvas, Point(0, 0), Point(300, 300), COLORNAME_GREEN);
    imshow("Canvas", canvas);
    waitKey();

    // Draw a 3-pixel thickness line from the top-right corner to the bottom-left
    line(canvas, Point(300, 0), Point(0, 300), COLORNAME_RED, 3);
    imshow("Canvas", canvas);
    waitKey();

    // Draw a green 50x50 pixel square, starting at (10, 10) and ending at (60, 60)
    rectangle(canvas, Point(10, 10), Point(60, 60), COLORNAME_GREEN);
    imshow("Canvas", canvas);
    waitKey();

    // Draw a 5-pixel thickness red rectangle
    rectangle(canvas, Point(50, 200), Point(200, 255), COLORNAME_RED, 5);
    imshow("Canvas", canvas);
    waitKey();

    // Draw a filled-in blue rectangle
    rectangle(canvas, Point(200, 50), Point(225, 125), COLORNAME_BLUE, FILLED);
    imshow("Canvas", canvas);
    waitKey();

    // Reset the canvas
    canvas = Mat::zeros(300, 300, CV_8UC3);

    // Draw a white circle at the center of the canvas with increasing radii, from 25 to 150 pixels
    int centerX = canvas.cols / 2;
    int centerY = canvas.rows / 2;
    for (unsigned int r = 0; r < 175; r+=25) {
        circle(canvas, Point(centerX, centerY), r, COLORNAME_WHITE);
    }
    imshow("Canvas", canvas);
    waitKey();

    // Draw 25 random circles
    srand(5);
    for (unsigned int i = 0; i < 25; i++) {
        // Generate a random radius size between 5 and 200
        int radius = rand() % 196 + 5;
        // Generate a random color
        Scalar color(rand() % 256, rand() % 256, rand() % 256);
        // Pick a random point as the center of a circle
        int cX = rand() % 300;
        int cY = rand() % 300;
        Point pt(cX, cY);
        // Draw a circle
        circle(canvas, pt, radius, color, FILLED);
    }
    imshow("Canvas", canvas);
    waitKey();

    return 0;
}
